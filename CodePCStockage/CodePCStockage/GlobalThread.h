//*********************************************************************************************
//* Programme : GlobalThread.cpp								Date : 18/03/2022
//*																Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Manage all classes to make them work together.
//*
//*********************************************************************************************
#pragma once

#include "Database.h"
#include <qthread.h>
#include <qmutex.h>
#include <qdebug.h>
#include "ManageConvoy.h"
#include <qcoreapplication.h>
#include <qtime>
#include "AllValuesSingleton.h"
#include <qqueue.h>
#include <signal.h>
#include "Scanner.h"
#include "TCPServer.h"
#include <QMap>
#include <qmap.h>
#include <QMultiMap>

enum ConveyorState
{
	NORMAL,
	WAIT_ELEVATOR1,
	WAIT_ELEVATOR2,
	WAIT_ELEVATOR3
};

class GlobalThread : public QThread
{
	Q_OBJECT

	ConveyorState state;
	QCoreApplication * app;
	QTime timer;
	QVector<bool> lastState;
	QVector<float> lastWeightValues;
	QVector<float> lastLengthValues;
	QVector<int> checkoutNumList;
	QVector<int> orderIdList;
	QVector<int> prescriptionIdList;
	QVector<float> weightList1;
	QVector<float> weightList2;
	QVector<float> weightList3;
	QVector<float> lengthList1;
	QVector<float> lengthList2;
	QVector<float> lengthList3;
	bool lastSensor3;

public:
	GlobalThread(QCoreApplication * app)
	{
		state = ConveyorState::NORMAL;
		this->app = app;
		timer.start();
	}

	static void manageQuit(int);
	static ManageConvoy * managerRef;

signals:
	void unwantedMedicine();
	void medicineNotScanned();

protected:

	virtual void run() override
	{
		srand(time(NULL));
		Database * db = new Database();
		db->start();
		ManageConvoy * manager = new ManageConvoy();
		TCPServer * server = new TCPServer(db, manager);
		Scanner * scanner = new Scanner(db);
		managerRef = manager;
		manager->connectToHost();
		bool isRunning = false;
		signal(SIGBREAK, GlobalThread::manageQuit);
		connect(this, SIGNAL(unwantedMedicine()), server, SLOT(unwantedMedicine()));
		connect(this, SIGNAL(medicineNotScanned()), server, SLOT(medicineNotScanned()));

		int readPeriod = 150;
		int nextRead = readPeriod;
		int clearConsolePeriod = 300;
		int nextClear = clearConsolePeriod;
		bool cylinder1State = false;
		bool cylinder2State = false;

		QVector<float> weightValues = AllValuesSingleton::getInstance()->getWeightSensors();
		QVector<float> lengthValues = AllValuesSingleton::getInstance()->getLengthSensors();

		lastState = AllValuesSingleton::getInstance()->getSensors();
		lastWeightValues = AllValuesSingleton::getInstance()->getWeightSensors();
		lastLengthValues = AllValuesSingleton::getInstance()->getLengthSensors();
		lastSensor3 = AllValuesSingleton::getInstance()->getSensor3();

		bool sensor3 = AllValuesSingleton::getInstance()->getSensor3();

		QQueue<int> cylinder1WaitingLine;
		QQueue<int> cylinder2WaitingLine;
		QQueue<int> extract1;
		QQueue<int> retract1;
		QQueue<int> extract2;
		QQueue<int> retract2;
		QQueue<int> aloneQueue;
		bool isTerminating = false;
		int elapsed = 0;

		//While we are connected to the automate
		while (manager->getConveyor()->getETZ512()->state() != QTcpSocket::UnconnectedState ) 
			//&& manager->getElevator()->getArduino()->getArduinoSocket()->state() == QTcpSocket::UnconnectedState)
		{
			//If the program is shutting down
			if (manager->isTermination())
			{
				manager->stopConveyor();
				manager->getConveyor()->getETZ512()->disconnectFromHost();
				disconnect(this, SIGNAL(unwantedMedicine()), server, SLOT(unwantedMedicine()));
				isTerminating = true;
			}
			//If the ptogram is running
			if (!isTerminating)
			{
				if (scanner->orderAvailable()) {
					//If the program is running without needing to send elevators we ontinue the timer and ask or values to the automate
					if (state == ConveyorState::NORMAL)
					{

						elapsed = timer.elapsed();
						if (elapsed >= nextRead)
						{
							manager->stateSensors();
							nextRead = elapsed + readPeriod;
						}
						if (elapsed >= nextClear)
						{
							system("cls");
							nextClear = elapsed + clearConsolePeriod;
						}


						QVector<bool> sensorsState = AllValuesSingleton::getInstance()->getSensors();
						sensor3 = AllValuesSingleton::getInstance()->getSensor3();
						AllValuesSingleton::getInstance()->setConveyor(isRunning);
						AllValuesSingleton::getInstance()->setCylinder1(cylinder1State);
						AllValuesSingleton::getInstance()->setCylinder2(cylinder2State);

						//If medicine is on the conveyor and the conveyor is not running we start the conveyor
						if (!isRunning) {
							manager->startConveyor();
							isRunning = true;
						}
						//if something is on the conveyor but didgn't get scanned
						if (!scanner->getMedicineScanned() && sensorsState[2] && sensorsState[2] != lastState[2]) {
							emit medicineNotScanned();
							weightList1.push_back(0.0);
							lengthList1.push_back(0.0);
							orderIdList.push_back(0.0);
							prescriptionIdList.push_back(0);
							checkoutNumList.push_back(1);
							cylinder1WaitingLine.push_back(1);
							int aloneResult = manager->checkAlone(1, 1);
							//Check which checkout sended its elevator and put a specific state for each checkout
							if (aloneResult == 1)
								state = WAIT_ELEVATOR1;
							aloneQueue.push_back(-1);
						}
						else if (scanner->getMedicineScanned() && sensorsState[2] && sensorsState[2] != lastState[2]) {
							scanner->setMedicineScanned(false);
						}
						//If something gets scanned
						if (scanner->getHasScanned()) {
							scanner->setHasScanned(false);
							QVector<QVariant> scannedMedInfo = scanner->identifyMed();
							if (scannedMedInfo.at(0) == -1) {
								emit unwantedMedicine();
								weightList1.push_back(0.0);
								lengthList1.push_back(0.0);
								orderIdList.push_back(0);
								prescriptionIdList.push_back(0);
								checkoutNumList.push_back(1);
								cylinder1WaitingLine.push_back(1);
								int aloneResult = manager->checkAlone(1, 1);
								//Check which checkout sended its elevator and put a specific state for each checkout
								if (aloneResult == 1)
									state = WAIT_ELEVATOR1;
								aloneQueue.push_back(-1);
							}
							else {
								//We push the checkout number in a list and organize the queues used to push the cylinders 
								orderIdList.push_back(scannedMedInfo.at(4).toInt());
								prescriptionIdList.push_back(scannedMedInfo.at(5).toInt());
								checkoutNumList.push_back(scannedMedInfo.at(1).toInt());
								
								if (checkoutNumList.back() == 1) {
									cylinder1WaitingLine.push_back(1);
									weightList1.push_back(scannedMedInfo.at(3).toFloat());
									lengthList1.push_back(scannedMedInfo.at(0).toFloat());
								}
								else if (checkoutNumList.back() == 2) {
									cylinder1WaitingLine.push_back(0);
									cylinder2WaitingLine.push_back(1);
									weightList2.push_back(scannedMedInfo.at(3).toFloat());
									lengthList2.push_back(scannedMedInfo.at(0).toFloat());
								}
								else if (checkoutNumList.back() == 3) {
									cylinder1WaitingLine.push_back(0);
									cylinder2WaitingLine.push_back(0);
									weightList3.push_back(scannedMedInfo.at(3).toFloat());
									lengthList3.push_back(scannedMedInfo.at(0).toFloat());
								}
								//Première valeur est le poids du medicament scanné. Le poids max d'une caisse est de 2.04 Check if a checkout need to send its elevator because of the weight
								int weightResult = manager->checkWeight(scannedMedInfo.at(3).toFloat(), checkoutNumList.back());
								//Première valeur est la taille du medicament scanné. La taille max d'une caisse est de 80.00 Check if a checkout need to send its elevator because of the length
								int lengthResult = manager->checkLength(scannedMedInfo.at(0).toFloat(), checkoutNumList.back());
								//We check which checkout sended its elevator and put a specific state for each checkout
								if (weightResult != 0 || lengthResult != 0)
								{
									if (weightResult == 1 || lengthResult == 1) {
										state = WAIT_ELEVATOR1;
										lengthValues = AllValuesSingleton::getInstance()->getLengthSensors();
										weightValues = AllValuesSingleton::getInstance()->getWeightSensors();
									}
									else if (weightResult == 2 || lengthResult == 2) {
										state = WAIT_ELEVATOR2;
										lengthValues = AllValuesSingleton::getInstance()->getLengthSensors();
										weightValues = AllValuesSingleton::getInstance()->getWeightSensors();
									}
									else if (weightResult == 3 || lengthResult == 3) {
										state = WAIT_ELEVATOR3;
										lengthValues = AllValuesSingleton::getInstance()->getLengthSensors();
										weightValues = AllValuesSingleton::getInstance()->getWeightSensors();
									}

								}

								//We check if the current medicine needs to be conveyed alone 
								int aloneResult = manager->checkAlone(scannedMedInfo.at(2).toBool(), checkoutNumList.back());
								//Check which checkout sended its elevator and put a specific state for each checkout
								if (aloneResult != 0) {
									if (aloneResult == 1) {
										state = WAIT_ELEVATOR1;
										aloneQueue.push_back(aloneResult);
									}
									else if (aloneResult == 2) {
										state = WAIT_ELEVATOR2;
										aloneQueue.push_back(aloneResult);
									}
									else if (aloneResult == 3) {
										state = WAIT_ELEVATOR3;
										aloneQueue.push_back(aloneResult);
									}
									else if (aloneResult == 4) {
										aloneQueue.push_back(1);
									}
									else if (aloneResult == 5) {
										aloneQueue.push_back(2);
									}
									else if (aloneResult == 6) {
										aloneQueue.push_back(3);
									}
									else if (aloneResult == 7) {
										aloneQueue.push_back(0);
									}
								}
							}
							

						}

						//If the sensor of the first cylinder changes state and the list of checkout isn't empty and the checkout is the number 1 we push a value that is equal to a time to wait before pushing or retracting a the cylinder
						if ((sensorsState[0] != lastState[0] && sensorsState[0]) && !cylinder1WaitingLine.isEmpty() && cylinder1WaitingLine.front() == 1)
						{
							extract1.push_back(elapsed + 300);
							retract1.push_back(elapsed + 550);
						}
						else if ((sensorsState[0] != lastState[0] && sensorsState[0]) && !cylinder1WaitingLine.isEmpty() && cylinder1WaitingLine.front() == 0) {
							cylinder1WaitingLine.pop_front();
						}

						//If the sensor of the second cylinder changes state and the list of checkout isn't empty and the checkout is the number 2 we push a value that is equal to a time to wait before pushing or retracting a the cylinder
						if ((sensorsState[1] != lastState[1] && sensorsState[1]) && !cylinder2WaitingLine.isEmpty() && cylinder2WaitingLine.front() == 1)
						{
							extract2.push_back(elapsed + 300);
							retract2.push_back(elapsed + 550);
						}
						else if ((sensorsState[1] != lastState[1] && sensorsState[1]) && !cylinder2WaitingLine.isEmpty() && cylinder2WaitingLine.front() == 0) {
							cylinder2WaitingLine.pop_front();

						}


						//If the extract queue's first cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and push the cylinder
						if (extract1.size() > 0 && elapsed > extract1[0])
						{
							extract1.pop_front();
							manager->pushCylinder(1);
							cylinder1State = true;
						}
						//If the extract queue's first cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and release the cylinder
						if (retract1.size() > 0 && elapsed > retract1[0])
						{
							
							for (int i = 0; i < checkoutNumList.size(); i++) {
								if (checkoutNumList.at(i) == 1) {
									checkoutNumList.removeAt(i);
									break;
								}
							}

							retract1.pop_front();
							cylinder1WaitingLine.pop_front();
							manager->releaseCylinder(1);
							cylinder1State = false;
							if (weightList1.front() != 0.0)
								scanner->incrementDeliveredQuantity(orderIdList.front(), prescriptionIdList.front());
							
							if (scanner->getEndOfOrder()) {
								scanner->setEndOfOrder(false);
								manager->sendElevator(1);
								state = WAIT_ELEVATOR1;
							}
							//If the medicine needs to be conveyed alone we send the elevator, stop the conveyor and change the state before deleting the value in the aloneQueue
							else if (!aloneQueue.isEmpty() && aloneQueue.front() == -1) {
								for (int i = 0; i < aloneQueue.size(); i++) {
									if (aloneQueue.at(i) == -1) {
										manager->sendElevator(1);
										state = WAIT_ELEVATOR1;
										aloneQueue.removeAt(i);
										break;
									}
								}

							}
							else if ((!aloneQueue.isEmpty() && aloneQueue.front() == 1)) {
								for (int i = 0; i < aloneQueue.size(); i++) {
									if (aloneQueue.at(i) == 1) {
										manager->sendElevator(aloneQueue.at(i));
										state = WAIT_ELEVATOR1;
										aloneQueue.removeAt(i);
										break;
									}
								}

							}
							else if (!aloneQueue.isEmpty() && aloneQueue.front() == 0) {
								aloneQueue.pop_front();
							}
							
							if (!weightList1.isEmpty() && (weightValues[0] + weightList1.front()) > 1000) {
								manager->sendElevator(1);
								state = WAIT_ELEVATOR1;
							}


							weightValues[0] += weightList1.front();
							AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
							weightList1.pop_front();
							
							if(!weightList1.isEmpty() && weightList1.front() == 0.0) {
								manager->sendElevator(1);
								state = WAIT_ELEVATOR1;
							}
							if (!lengthList1.isEmpty() && (lengthValues[0] + lengthList1.front()) > 80.00) {
								manager->sendElevator(1);
								state = WAIT_ELEVATOR1;
								
							}

							lengthValues[0] += lengthList1.front();
							AllValuesSingleton::getInstance()->setLengthSensors(lengthValues[0], lengthValues[1], lengthValues[2]);
							lengthList1.pop_front();

							if (!aloneQueue.isEmpty() && aloneQueue.front() == 1) {
								manager->sendElevator(1);
								state = WAIT_ELEVATOR1;
							}

							
							orderIdList.pop_front();
							prescriptionIdList.pop_front();
							
						}

						
						//If the extract queue's second cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and push the cylinder
						if (extract2.size() > 0 && elapsed > extract2[0])
						{
							extract2.pop_front();
							manager->pushCylinder(2);
							cylinder2State = true;
						}
						//If the extract queue's second cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and release the cylinder
						if (retract2.size() > 0 && elapsed > retract2[0])
						{
							for (int i = 0; i < checkoutNumList.size(); i++) {
								if (checkoutNumList.at(i) == 2) {
									checkoutNumList.removeAt(i);
									break;
								}
							}
							
							retract2.pop_front();
							cylinder2WaitingLine.pop_front();
							manager->releaseCylinder(2);
							cylinder2State = false;
							scanner->incrementDeliveredQuantity(orderIdList.front(), prescriptionIdList.front());
							if (scanner->getEndOfOrder()) {
								scanner->setEndOfOrder(false);
								manager->sendElevator(2);
								state = WAIT_ELEVATOR2;
							}
							//If the medicine needs to be conveyed alone we send the elevator, stop the conveyor and change the state before deleting the value in the aloneQueue
							else if (aloneQueue.size() > 0) {
								for (int i = 0; i < aloneQueue.size(); i++) {
									if (aloneQueue.at(i) == 2) {
										manager->sendElevator(aloneQueue.at(i));
										state = WAIT_ELEVATOR2;
										aloneQueue.removeAt(i);
										break;
									}
								}
							}
							orderIdList.pop_front();
							prescriptionIdList.pop_front();

							if (!weightList2.isEmpty() && (weightValues[1] + weightList2.front()) > 1000) {
								manager->sendElevator(2);
								state = WAIT_ELEVATOR2;
							}
							weightValues[1] += weightList2.front();
							AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
							weightList2.pop_front();
							if (!lengthList2.isEmpty() && (lengthValues[1] + lengthList2.front()) > 80.00) {
								manager->sendElevator(2);
								state = WAIT_ELEVATOR2;
							}
							lengthValues[1] += lengthList2.front();
							AllValuesSingleton::getInstance()->setLengthSensors(lengthValues[0], lengthValues[1], lengthValues[2]);
							lengthList2.pop_front();

							if (!aloneQueue.isEmpty() && aloneQueue.front() == 2) {
								manager->sendElevator(2);
								state = WAIT_ELEVATOR2;
							}
						}

						

						//When the medicine fall in the elevator we substract it from the value of current medicine on the conveyor
						///Changer le esle if avec le capteur de présence pour detecter un medicament vers la caisse trois
						else if (sensor3 != lastSensor3 && sensor3 && !checkoutNumList.isEmpty() && !weightList3.isEmpty()) {
							scanner->incrementDeliveredQuantity(orderIdList.front(), prescriptionIdList.front());
							if (scanner->getEndOfOrder()) {
								scanner->setEndOfOrder(false);
								manager->sendElevator(3);
								state = WAIT_ELEVATOR3;
							}
							//If the medicine needs to be conveyed alone we send the elevator, stop the conveyor and change the state before deleting the value in the aloneQueue
							else if (aloneQueue.size() > 0) {
								for (int i = 0; i < aloneQueue.size(); i++) {
									if (aloneQueue.at(i) == 3) {
										manager->sendElevator(aloneQueue.at(i));
										state = WAIT_ELEVATOR3;
										aloneQueue.removeAt(i);
										break;
									}
								}
							}
							orderIdList.pop_front();
							prescriptionIdList.pop_front();

							if (!weightList3.isEmpty() && (weightValues[2] + weightList3.front()) > 1000) {
								manager->sendElevator(3);
								state = WAIT_ELEVATOR3;
							}
							weightValues[2] += weightList3.front();
							AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
							weightList3.pop_front();
							if (!lengthList3.isEmpty() && (lengthValues[2] + lengthList3.front()) >= 80.01) {
								manager->sendElevator(3);
								state = WAIT_ELEVATOR3;
							}
							lengthValues[2] += lengthList3.front();
							AllValuesSingleton::getInstance()->setLengthSensors(lengthValues[0], lengthValues[1], lengthValues[2]);
							lengthList3.pop_front();
							
							for (int i = 0; i < checkoutNumList.size(); i++) {
								if (checkoutNumList.at(i) == 3) {
									checkoutNumList.removeAt(i);
									break;
								}
							}
							
							if (!aloneQueue.isEmpty() && aloneQueue.front() == 3) {
								manager->sendElevator(3);
								state = WAIT_ELEVATOR3;
							}
							
						}
						//We stock the new values in a vector that will permit to each time compare the last values with the ones from the sensors
						lastState = sensorsState;
						lastWeightValues = weightValues;
						lastLengthValues = lengthValues;
						lastSensor3 = sensor3;

					}
					//If we are in the state WAIT_ELEVATOR1 the program will be paused until the elevator 1 ascend to deliver medicine and descend back in its original position
					else if (state == ConveyorState::WAIT_ELEVATOR1)
					{
					if (isRunning) {
						manager->stopConveyor();
						isRunning = false;
						AllValuesSingleton::getInstance()->setConveyor(isRunning);
					}
					AllValuesSingleton::getInstance()->setCylinder1(cylinder1State);
						//Retrieve the states of the elevator 1 
						QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
						//While the elevator is not here
						while (elevatorStates[0]) {
							if (elapsed >= nextClear)
							{
								system("cls");
								nextClear = elapsed + clearConsolePeriod;
							}
							//Retrieve the values of buttons linked to the state of the elevators. The button will be a simulation of the pharmacist sending back the elevator
							QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
							//When the button is pressed we restart the conveyor and come back in the NORMAL state
							if (elevatorButtons[0]) {
								//if (scanner->orderAvailable())
								//	manager->startConveyor();
								state = ConveyorState::NORMAL;
								weightValues[0] = 0;
								lengthValues[0] = 0;
								AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
								AllValuesSingleton::getInstance()->setLengthSensors(lengthValues[0], lengthValues[1], lengthValues[2]);
								manager->display();
								app->processEvents();
								break;
							}
							manager->display();
							app->processEvents();

						}
						manager->display();
						app->processEvents();
					}
					//If we are in the state WAIT_ELEVATOR2 the program will be paused until the elevator 2 ascend to deliver medicine and descend back in its original position
					else if (state == ConveyorState::WAIT_ELEVATOR2)
					{
					if (isRunning) {
						manager->stopConveyor();
						isRunning = false;
						AllValuesSingleton::getInstance()->setConveyor(isRunning);
					}
					AllValuesSingleton::getInstance()->setCylinder2(cylinder2State);
						//Retrieve the states of the elevator 2 
						QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
						//While the elevator is not here
						while (elevatorStates[1]) {
							if (elapsed >= nextClear)
							{
								system("cls");
								nextClear = elapsed + clearConsolePeriod;
							}
							//Retrieve the values of buttons linked to the state of the elevators. The button will be a simulation of the pharmacist sending back the elevator
							QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
							//When the button is pressed we restart the conveyor and come back in the NORMAL state
							if (elevatorButtons[1]) {
								//if (scanner->orderAvailable())
								//	manager->startConveyor();
								state = ConveyorState::NORMAL;
								weightValues[1] = 0;
								lengthValues[1] = 0;
								AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
								AllValuesSingleton::getInstance()->setLengthSensors(lengthValues[0], lengthValues[1], lengthValues[2]);
								manager->display();
								app->processEvents();
								break;
							}
							
							manager->display();
							app->processEvents();
						}
						manager->display();
						app->processEvents();

					}
					//If we are in the state WAIT_ELEVATOR2 the program will be paused until the elevator 2 ascend to deliver medicine and descend back in its original position
					else if (state == ConveyorState::WAIT_ELEVATOR3)
					{
					if (isRunning) {
						manager->stopConveyor();
						isRunning = false;
						AllValuesSingleton::getInstance()->setConveyor(isRunning);
					}
						//Retrieve the states of the elevator 2 
						QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
						//While the elevator is not here
						while (elevatorStates[2]) {
							if (elapsed >= nextClear)
							{
								system("cls");
								nextClear = elapsed + clearConsolePeriod;
							}
							//Retrieve the values of buttons linked to the state of the elevators. The button will be a simulation of the pharmacist sending back the elevator
							QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
							//When the button is pressed we restart the conveyor and come back in the NORMAL state
							if (elevatorButtons[2]) {
								//if (scanner->orderAvailable())
								//	manager->startConveyor();
								state = ConveyorState::NORMAL;
								weightValues[2] = 0;
								lengthValues[2] = 0;
								AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
								AllValuesSingleton::getInstance()->setLengthSensors(lengthValues[0], lengthValues[1], lengthValues[2]);
								manager->display();
								app->processEvents();
								break;
							}
							manager->display();
							app->processEvents();
						}
						manager->display();
						app->processEvents();

					}
				}
				else {
					//If ther is no more medicine and the conveyor is running we stop the conveyor
					if (isRunning) {
						manager->stopConveyor();
						isRunning = false;
					}
					state = NORMAL;
					manager->display();
					app->processEvents();
					//Le cas du dernier med des commandes dispo ne rentre pas dans les states wait elevator et donc ne redescend pas
				}
				
			}
			manager->display();
			app->processEvents();
		}

		qDebug() << "End of thread";
	}


};


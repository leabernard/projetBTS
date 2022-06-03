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
	QVector<int> checkoutNumList;
	QVector<int> orderIdList;
	int medCount = 0;

public:
	GlobalThread(QCoreApplication * app)
	{
		state = ConveyorState::NORMAL;
		this->app = app;
		timer.start();
	}

	static void manageQuit(int);
	static ManageConvoy * managerRef;

protected:

	virtual void run() override
	{
		srand(time(NULL));
		Database * db = new Database();
		db->start();
		ManageConvoy * manager = new ManageConvoy();
		Scanner * scanner = new Scanner(db);
		managerRef = manager;
		manager->connectToHost();
		bool isRunning = false;
		signal(SIGBREAK, GlobalThread::manageQuit);


		int readPeriod = 150;
		int nextRead = readPeriod;

		lastState = AllValuesSingleton::getInstance()->getSensors();
		lastWeightValues = AllValuesSingleton::getInstance()->getWeightSensors();

		QQueue<int> cylinder1WaitingLine;
		QQueue<int> cylinder2WaitingLine;
		QQueue<int> extract1;
		QQueue<int> retract1;
		QQueue<int> extract2;
		QQueue<int> retract2;
		QQueue<int> aloneQueue;
		bool isTerminating = false;

		//While we are connected to the automate
		while (manager->getConveyor()->getETZ512()->state() != QTcpSocket::UnconnectedState ) 
			//&& manager->getElevator()->getArduino()->getArduinoSocket()->state() == QTcpSocket::UnconnectedState)
		{
			//If the program is shutting down
			if (manager->isTermination())
			{
				manager->stopConveyor();
				manager->getConveyor()->getETZ512()->disconnectFromHost();
				isTerminating = true;
			}
			//If the ptogram is running
			if (!isTerminating)
			{
				//If the program is running without needing to send elevators we ontinue the timer and ask or values to the automate
				if (state == ConveyorState::NORMAL)
				{

					int elapsed = timer.elapsed();
					if (elapsed >= nextRead)
					{
						manager->stateSensors();
						nextRead = elapsed + readPeriod;
					}


					QVector<bool> sensorsState = AllValuesSingleton::getInstance()->getSensors();
					QVector<float> weightValues = AllValuesSingleton::getInstance()->getWeightSensors();

					//If something gets scanned
					if (scanner->getHasScanned()) {
						scanner->setHasScanned(false);
						QVector<QVariant> scannedMedInfo = scanner->identifyMed(scanner->getBarCode());
						//We push the checkout number in a list and organize the queues used to push the cylinders 
						orderIdList.push_back(scannedMedInfo.at(4).toInt());
						checkoutNumList.push_back(scannedMedInfo.at(1).toInt());
						if (checkoutNumList.front() == 1) {
							cylinder1WaitingLine.push_back(1);
						}
						else if (checkoutNumList.front() == 2) {
							cylinder1WaitingLine.push_back(0);
							cylinder2WaitingLine.push_back(1);
						}
						else if (checkoutNumList.front() == 3) {
							cylinder1WaitingLine.push_back(0);
							cylinder2WaitingLine.push_back(0);
						}
						//Première valeur est le poids du medicament scanné. Le poids max d'une caisse est de 2.04 Check if a checkout need to send its elevator because of the weight
						int weightResult = manager->checkWeight(scannedMedInfo.at(3).toFloat(), checkoutNumList.front());
						//Première valeur est la taille du medicament scanné. La taille max d'une caisse est de 80.00 Check if a checkout need to send its elevator because of the length
						int lengthResult = manager->checkLength(scannedMedInfo.at(0).toFloat(), checkoutNumList.front());
						//We check which checkout sended its elevator and put a specific state for each checkout
						if (weightResult != 0 || lengthResult != 0)
						{
							if (weightResult == 1 || lengthResult == 1) {
								state = WAIT_ELEVATOR1;
							}
							else if (weightResult == 2 || lengthResult == 2) {
								state = WAIT_ELEVATOR2;
							}
							else if (weightResult == 3 || lengthResult == 3) {
								state = WAIT_ELEVATOR3;
							}
							
						}

						//We check if the current medicine needs to be conveyed alone 
						int aloneResult = manager->checkAlone(scannedMedInfo.at(2).toBool(), checkoutNumList.front());
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
						}

					}

					if (sensorsState[2] != lastState[2] && sensorsState[2])
						medCount++;
					
					//If medicine is on the conveyor and the conveyor is not running we start the conveyor
					if (medCount > 0 && !isRunning) {
						manager->startConveyor();
						isRunning = true;
					}

					//If ther is no more medicine and the conveyor is running we sotp the conveyor
					if (medCount <= 0 && isRunning) {
						manager->stopConveyor();
						isRunning = false;
					}

					//If the sensor of the first cylinder changes state and the list of checkout isn't empty and the checkout is the number 1 we push a value that is equal to a time to wait before pushing or retracting a the cylinder
					if ((sensorsState[0] != lastState[0] && sensorsState[0]) && !cylinder1WaitingLine.isEmpty() && cylinder1WaitingLine.front() == 1)
					{
						extract1.push_back(elapsed + 100);
						retract1.push_back(elapsed + 350);
					}
					else if ((sensorsState[0] != lastState[0] && sensorsState[0]) && !cylinder1WaitingLine.isEmpty() && cylinder1WaitingLine.front() == 0) {
						cylinder1WaitingLine.pop_front();
					}

					//If the sensor of the second cylinder changes state and the list of checkout isn't empty and the checkout is the number 2 we push a value that is equal to a time to wait before pushing or retracting a the cylinder
					if ((sensorsState[1] != lastState[1] && sensorsState[1]) && !cylinder2WaitingLine.isEmpty() && cylinder2WaitingLine.front() == 1)
					{
						extract2.push_back(elapsed + 100);
						retract2.push_back(elapsed + 350);
					}
					else if ((sensorsState[1] != lastState[1] && sensorsState[1]) && !cylinder2WaitingLine.isEmpty() && cylinder2WaitingLine.front() == 0) {
						cylinder2WaitingLine.pop_front();

					}


					//If the extract queue's first cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and push the cylinder
					if (extract1.size() > 0 && elapsed > extract1[0])
					{
						extract1.pop_front();
						manager->pushCylinder(1);
					}
					//If the extract queue's first cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and release the cylinder
					if (retract1.size() > 0 && elapsed > retract1[0])
					{
						retract1.pop_front();
						checkoutNumList.pop_front();
						cylinder1WaitingLine.pop_front();
						manager->releaseCylinder(1);
						medCount--;
						//If the medicine needs to be conveyed alone we send the elevator, stop the conveyor and change the state before deleting the value in the aloneQueue
						if (aloneQueue.front() == 1 && aloneQueue.size() > 0) {
							manager->getArduino()->sendElevator(aloneQueue.front());
							manager->stopConveyor();
							state = WAIT_ELEVATOR1;
							aloneQueue.pop_front();
						}
					}

					//If the extract queue's second cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and push the cylinder
					if (extract2.size() > 0 && elapsed > extract2[0])
					{
						extract2.pop_front();
						manager->pushCylinder(2);
					}
					//If the extract queue's second cylinder isn't empty and the value is smaller than the time elapsed we delete the value from the queue and release the cylinder
					if (retract2.size() > 0 && elapsed > retract2[0])
					{
						checkoutNumList.pop_front();
						retract2.pop_front();
						cylinder2WaitingLine.pop_front();
						manager->releaseCylinder(2);
						medCount--;
						//If the medicine needs to be conveyed alone we send the elevator, stop the conveyor and change the state before deleting the value in the aloneQueue
						if (aloneQueue.front() == 2 && aloneQueue.size() > 0) {
							manager->sendElevator(aloneQueue.front());
							manager->stopConveyor();
							state = WAIT_ELEVATOR2;
							aloneQueue.pop_front();
						}
					}
					//When the medicine fall in the elevator we substract it from the value of current medicine on the conveyor
					else if ((weightValues[2] > lastWeightValues[2]) && medCount > 0) {
						medCount--;
						//If the checkoutNumList isn't empty we delete the value of the curre checkout
						if(!checkoutNumList.isEmpty())
							checkoutNumList.pop_front();
						//If the medicine needs to be conveyed alone we send the elevator, stop the conveyor and change the state before deleting the value in the aloneQueue
						if (aloneQueue.front() == 3 && aloneQueue.size() > 0) {
							manager->getArduino()->sendElevator(aloneQueue.front());
							manager->stopConveyor();
							state = WAIT_ELEVATOR3;
							aloneQueue.pop_front();
						}
					}
					//We We stock the new values in a vector that will permit to each time compare the last values with the ones from the sensors
					lastState = sensorsState;
					lastWeightValues = weightValues;

				}
				//If we are in the state WAIT_ELEVATOR1 the program will be paused until the elevator 1 ascend to deliver medicine and descend back in its original position
				else if (state == ConveyorState::WAIT_ELEVATOR1)
				{
					//Retrieve the states of the elevator 1 
					QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
					//While the elevator is not here
					while (elevatorStates[0]) {
						//Retrieve the values of buttons linked to the state of the elevators. The button will be a simulation of the pharmacist sending back the elevator
						QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
						//When the button is pressed we restart the conveyor and come back in the NORMAL state
						if (elevatorButtons[0]) {
							if(medCount != 0)
								manager->startConveyor();
							state = ConveyorState::NORMAL;
							break;
						}
						manager->display();
						app->processEvents();
							
					}

				}
				//If we are in the state WAIT_ELEVATOR2 the program will be paused until the elevator 2 ascend to deliver medicine and descend back in its original position
				else if (state == ConveyorState::WAIT_ELEVATOR2)
				{
					//Retrieve the states of the elevator 2 
					QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
					//While the elevator is not here
					while (elevatorStates[1]) {
						//Retrieve the values of buttons linked to the state of the elevators. The button will be a simulation of the pharmacist sending back the elevator
						QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
						//When the button is pressed we restart the conveyor and come back in the NORMAL state
						if (elevatorButtons[1]) {
							if (medCount != 0)
								manager->startConveyor();
							state = ConveyorState::NORMAL;
							break;
						}
						manager->display();
						app->processEvents();
					}

				}
				//If we are in the state WAIT_ELEVATOR2 the program will be paused until the elevator 2 ascend to deliver medicine and descend back in its original position
				else if (state == ConveyorState::WAIT_ELEVATOR3)
				{
				//Retrieve the states of the elevator 2 
					QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
					//While the elevator is not here
					while (elevatorStates[2]) {
						//Retrieve the values of buttons linked to the state of the elevators. The button will be a simulation of the pharmacist sending back the elevator
						QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
						//When the button is pressed we restart the conveyor and come back in the NORMAL state
						if (elevatorButtons[2]) {
							if (medCount != 0)
								manager->startConveyor();
							state = ConveyorState::NORMAL;
							break;
						}
						manager->display();
						app->processEvents();
					}

				}
			}
			manager->display();
			app->processEvents();
		}

		qDebug() << "End of thread";
	}


};


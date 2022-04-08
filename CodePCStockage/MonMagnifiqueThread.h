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

#include <qthread.h>
#include <qmutex.h>
#include <qdebug.h>
#include "ManageConvoy.h"
#include <qcoreapplication.h>
#include <qtime>
#include "AllValuesSingleton.h"
#include <qqueue.h>
#include <signal.h>
#include "reader.h"

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
	QTime timer2;
	QVector<bool> lastState;
	QVector<float> lastWeightValues;
	QVector<int> checkoutNumList;
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

		ManageConvoy * manager = new ManageConvoy();
		Reader * reader = new Reader();
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

		while (manager->getConveyor()->getETZ512()->state() != QTcpSocket::UnconnectedState ) 
			//&& manager->getElevator()->getArduino()->getArduinoSocket()->state() == QTcpSocket::UnconnectedState)
		{
			if (manager->isTermination())
			{
				manager->stopConveyor();
				manager->getConveyor()->getETZ512()->disconnectFromHost();
				isTerminating = true;
			}

			if (!isTerminating)
			{
				if (state == ConveyorState::NORMAL)
				{

					int elapsed = timer.elapsed();
					int elapsedWrite = timer2.elapsed();
					if (elapsed >= nextRead)
					{
						manager->stateSensors();
						nextRead = elapsed + readPeriod;
					}


					QVector<bool> sensorsState = AllValuesSingleton::getInstance()->getSensors();
					QVector<float> weightValues = AllValuesSingleton::getInstance()->getWeightSensors();

					if (reader->getScanned()) {
						reader->setScanned(false);
						//Une liste des numéros de caisse de avec comme valeurs de 1 a 3
						checkoutNumList.push_back(rand() % 3 + 1);
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
						//Première valeur est le poids du medicament scanné. Le poids max d'une caisse est de 2.04
						//int weightResult = manager->checkWeight(2.5, checkoutNumList.front());
						//Première valeur est la taille du medicament scanné. La taille max d'une caisse est de 80.00
						//int lengthResult = manager->checkLength(85.0, checkoutNumList.front());
						/*if (weightResult != 0 || lengthResult != 0)
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
							
						}*/
						
						int aloneResult = manager->checkAlone(1, checkoutNumList.front());
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

					if (medCount > 0 && !isRunning) {
						manager->startConveyor();
						isRunning = true;
					}

					if (medCount <= 0 && isRunning) {
						manager->stopConveyor();
						isRunning = false;
					}


					if ((sensorsState[0] != lastState[0] && sensorsState[0]) && !checkoutNumList.isEmpty() && checkoutNumList.front() == 1)
					{
						extract1.push_back(elapsed + 100);
						retract1.push_back(elapsed + 350);
					}

					if ((sensorsState[1] != lastState[1] && sensorsState[1]) && !checkoutNumList.isEmpty() && checkoutNumList.front() == 2)
					{
						extract2.push_back(elapsed + 100);
						retract2.push_back(elapsed + 350);
					}



					if (extract1.size() > 0 && elapsed > extract1[0])
					{
						extract1.pop_front();
						manager->pushCylinder(1);
					}

					if (retract1.size() > 0 && elapsed > retract1[0])
					{
						retract1.pop_front();
						checkoutNumList.pop_front();
						manager->releaseCylinder(1);
						if (aloneQueue.front() == 1 && aloneQueue.size() > 0) {
							manager->getElevator()->getArduino()->sendElevator(aloneQueue.front());
							manager->stopConveyor();
							state = WAIT_ELEVATOR1;
							aloneQueue.pop_front();
						}
					}

					if (extract2.size() > 0 && elapsed > extract2[0])
					{
						extract2.pop_front();
						manager->pushCylinder(2);
					}

					if (retract2.size() > 0 && elapsed > retract2[0])
					{
						checkoutNumList.pop_front();
						retract2.pop_front();
						manager->releaseCylinder(2);
						if (aloneQueue.front() == 2 && aloneQueue.size() > 0) {
							manager->getElevator()->getArduino()->sendElevator(aloneQueue.front());
							manager->stopConveyor();
							state = WAIT_ELEVATOR2;
							aloneQueue.pop_front();
						}
					}

					if ((weightValues[0] > lastWeightValues[0] || weightValues[1] > lastWeightValues[1])) {
						medCount--;

					}
					else if ((weightValues[2] > lastWeightValues[2]) && medCount > 0) {
						medCount--;
						if(!checkoutNumList.isEmpty())
							checkoutNumList.pop_front();
						if (aloneQueue.front() == 3 && aloneQueue.size() > 0) {
							manager->getElevator()->getArduino()->sendElevator(aloneQueue.front());
							manager->stopConveyor();
							state = WAIT_ELEVATOR3;
							aloneQueue.pop_front();
						}
					}

					lastState = sensorsState;
					lastWeightValues = weightValues;

				}
				else if (state == ConveyorState::WAIT_ELEVATOR1)
				{
					QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
					while (elevatorStates[0]) {
						QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
						if (elevatorButtons[0]) {
							manager->startConveyor();
							state = ConveyorState::NORMAL;
							break;
						}
						app->processEvents();
							
					}

				}
				else if (state == ConveyorState::WAIT_ELEVATOR2)
				{
					QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
					while (elevatorStates[1]) {
						QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
						if (elevatorButtons[1]) {
							manager->startConveyor();
							state = ConveyorState::NORMAL;
							break;
						}
						app->processEvents();
					}

				}
				else if (state == ConveyorState::WAIT_ELEVATOR3)
				{
					QVector<bool> elevatorStates = AllValuesSingleton::getInstance()->getElevatorState();
					while (elevatorStates[2]) {
						QVector<bool> elevatorButtons = AllValuesSingleton::getInstance()->getElevatorButton();
						if (elevatorButtons[2]) {
							manager->startConveyor();
							state = ConveyorState::NORMAL;
							break;
						}
						app->processEvents();
					}

				}
			}

			app->processEvents();
		}

		qDebug() << "End of thread";
	}


};


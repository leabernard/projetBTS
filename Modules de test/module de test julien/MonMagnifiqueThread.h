#pragma once

#include <qthread.h>
#include <qmutex.h>
#include <qdebug.h>
#include "ManageConvoy.h"
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
#include <qcoreapplication.h>
#include <qtime>
#include "AllValuesSingleton.h"
#include <qqueue.h>
#include <signal.h>
#include "Reader.h"

class GlobalThread : public QThread
{
	Q_OBJECT

	QCoreApplication * app;
	QTime timer;
	QTime timer2;
	QVector<bool> lastState;

public:
	GlobalThread(QCoreApplication * app)
	{
		this->app = app;
		timer.start();
		timer2.start();
	}

	static void manageQuit(int);
	static ManageConvoy * managerRef;

protected:

	virtual void run() override
	{
		Reader * scannerData = new Reader();
		ManageConvoy * manager = new ManageConvoy(app);
		managerRef = manager;
		manager->connectToHost();
		manager->startConveyor();
		signal(SIGBREAK, GlobalThread::manageQuit);


		int readPeriod = 150;
		int writePeriod = 200;
		int nextRead = readPeriod;
		int nextWrite = writePeriod;

		lastState = AllValuesSingleton::getInstance()->getSensors();


		QQueue<int> extract1;
		QQueue<int> retract1;
		QQueue<int> extract2;
		QQueue<int> retract2;
		bool isTerminating = false;

		while (manager->getConveyor()->getETZ512()->state() != QTcpSocket::UnconnectedState)
		{
			if (manager->isTermination())
			{
				manager->stopConveyor();
				manager->getConveyor()->getETZ512()->disconnectFromHost();
				isTerminating = true;
			}

			if (!isTerminating)
			{
				int elapsed = timer.elapsed();
				int elapsedWrite = timer2.elapsed();
				if (elapsed >= nextRead)
				{
					manager->stateSensors();
					nextRead = elapsed + readPeriod;
				}


				QVector<bool> sensorsState = AllValuesSingleton::getInstance()->getSensors();

				if (sensorsState[0] != lastState[0] && sensorsState[0])
				{
					extract1.push_back(elapsed + 100);
					retract1.push_back(elapsed + 350);
				}

				if (sensorsState[1] != lastState[1] && sensorsState[1])
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
					manager->releaseCylinder(1);
				}

				if (extract2.size() > 0 && elapsed > extract2[0])
				{
					extract2.pop_front();
					manager->pushCylinder(2);
				}

				if (retract2.size() > 0 && elapsed > retract2[0])
				{
					retract2.pop_front();
					manager->releaseCylinder(2);
				}

				lastState = sensorsState;
			}

			app->processEvents();
		}

		qDebug() << "End of thread";
	}


};


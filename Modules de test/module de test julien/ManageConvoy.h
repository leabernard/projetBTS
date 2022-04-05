//*********************************************************************************************
//* Programme : ManageConvoy.cpp								Date : 18/03/2022
//*																Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Manage all sub-classes.
//*
//*********************************************************************************************
#pragma once

#include <QObject>
#include "Conveyor.h"
#include "Cylinder.h"
#include "Elevator.h"
#include "AllValuesSingleton.h"
#include <qcoreapplication.h>
#include <qmutex.h>
#include <qsemaphore.h>

class ManageConvoy : public QObject
{
	Q_OBJECT

private:
	QCoreApplication * app;
	Conveyor * conveyor;
	Cylinder * cylinders;
	Elevator * elevator;
	int checkoutNum;
	QList<bool> sensorsDetectionArray;
	AllValuesSingleton * instance;
	QSemaphore * endSemaphore;
	bool termination;

public:
	ManageConvoy(QCoreApplication * app, QObject *parent = Q_NULLPTR);
	~ManageConvoy();
	void startConveyor();
	void stopConveyor();
	void connectToHost();
	void pushCylinder(int checkoutNum);
	void releaseCylinder(int checkoutNum);
	void checkWeight(int weight);
	void checkLength(int length);
	void sendElevator(int checkoutNum);
	void checkAlone(bool alone);
	void stateSensors();
	void stateWeightSensors();
	void stateElevator();
	void display();
	Conveyor * getConveyor();
	void disconnectHost();

	bool isTermination() {
		return termination;
	}

	void getTerminationLock()
	{
		qDebug() << "Acquire";
		endSemaphore->acquire(1);
		qDebug() << "Acquire OK";
	}

	void releaseTerminationLock()
	{
		qDebug() << "Release";
		endSemaphore->release(1);
		qDebug() << "Release OK";
	}

public slots:
	void receiveSensorsValues(quint16 address, QVector<quint16> values);
	void onConveyorDisconnected();
	
};

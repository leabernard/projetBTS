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
#include "AllValuesSingleton.h"
#include <qcoreapplication.h>
#include <qmutex.h>
#include <qsemaphore.h>
#include "Arduino.h"

class ManageConvoy : public QObject
{
	Q_OBJECT

private:
	Conveyor * conveyor;
	Cylinder * cylinders;
	Arduino * arduino;
	int checkoutNum;
	AllValuesSingleton * instance;
	QSemaphore * endSemaphore;
	bool termination;

public:
	ManageConvoy(QObject *parent = Q_NULLPTR);
	~ManageConvoy();
	void startConveyor();
	void stopConveyor();
	void connectToHost();
	void pushCylinder(int checkoutNum);
	void releaseCylinder(int checkoutNum);
	int checkWeight(float weight, int checkoutNum);
	int checkLength(float length, int checkoutNum);
	int sendElevator(int checkoutNum);
	int checkAlone(bool alone, int checkoutNum);
	void stateSensors();
	void display();
	Conveyor * getConveyor();
	Arduino * getArduino();
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

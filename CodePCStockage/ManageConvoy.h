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

class ManageConvoy : public QObject
{
	Q_OBJECT

private:
	Conveyor * conveyor;
	Cylinder * cylinders;
	Elevator * elevator;
	int checkoutNum;
	QList<bool> sensorsDetectionArray;
public:
	ManageConvoy(QObject *parent = Q_NULLPTR);
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

};

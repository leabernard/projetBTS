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
#include "ManageConvoy.h"

ManageConvoy::ManageConvoy(QObject *parent)
	: QObject(parent)
{
	conveyor = new Conveyor();
}

ManageConvoy::~ManageConvoy()
{
}
//Method to start the conveyor and write the change in console log
void ManageConvoy::startConveyor()
{
	//if we are connected we can continue
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		//Method to start the conveyor 
		conveyor->startConveyor();
	}
}
//Method to stop the conveyor and write the change in console log
void ManageConvoy::stopConveyor()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		//Method to stop the conveyor 
		conveyor->stopConveyor();
	}
}
//Method to connect to the ETZ512 card
void ManageConvoy::connectToHost()
{
	if (conveyor->connectToModbus() == true) {
		qDebug() << "Connected";
	} else {
		qDebug() << "Not connected";
	}
}

void ManageConvoy::pushCylinder(int checkoutNum)
{
}

void ManageConvoy::releaseCylinder(int checkoutNum)
{
}

void ManageConvoy::checkWeight(int weight)
{
}

void ManageConvoy::checkLength(int length)
{
}

void ManageConvoy::sendElevator(int checkoutNum)
{
}

void ManageConvoy::checkAlone(bool alone)
{
}

void ManageConvoy::stateSensors()
{
}

void ManageConvoy::stateWeightSensors()
{
}

void ManageConvoy::stateElevator()
{
}

void ManageConvoy::display()
{
}

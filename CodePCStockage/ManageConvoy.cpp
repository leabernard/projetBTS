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
	elevator = new Elevator();
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
		qDebug() << "tapis ON";
	}
}
//Method to stop the conveyor and write the change in console log
void ManageConvoy::stopConveyor()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		//Method to stop the conveyor 
		conveyor->stopConveyor();
		qDebug() << "tapis OFF";
	}
}
//Method to connect to the ETZ512 card
void ManageConvoy::connectToHost()
{
	if (conveyor->connectToModbus() == true) {
		connect(conveyor->getETZ512(), SIGNAL(onReadMultipleHoldingRegistersSentenceSingleValue(quint16, quint16)), this, SLOT(testSensors(quint16, quint16)));
		cylinders = new Cylinder(conveyor->getETZ512());
		qDebug() << "Connected";
	} else {
		qDebug() << "Not connected";
	}
}

void ManageConvoy::pushCylinder(int checkoutNum)
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		cylinders->pushCylinder(checkoutNum);
		qDebug() << "Verin " + QString::number(checkoutNum) + " active";
	}
}

void ManageConvoy::releaseCylinder(int checkoutNum)
{
	if (cylinders->getETZ512()->state() == QTcpSocket::ConnectedState) {
		cylinders->releaseCylinder(checkoutNum);
		qDebug() << "Verin " + QString::number(checkoutNum) + " desactive";
	}
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
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		conveyor->stateSensors();
	}
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

void ManageConvoy::receiveSensorsValues(quint16 address, quint16 value) {
	float value1;
	float value2;
	float value3;
	if (address == conveyor->getSensor1()) {
		qDebug() << "Capteur 1 : " + QString::number(value);
		value1 = value;
	}
	if (address == conveyor->getSensor2()) {
		qDebug() << "Capteur 2 : " + QString::number(value);
		value2 = value;
	}
	if (address == conveyor->getSensorScanner()) {
		qDebug() << "Capteur Scanner : " + QString::number(value);
		value3 = value;
	}
	instance = AllValuesSingleton::getInstance();
	instance->setSensors(value1, value2, value3);
}

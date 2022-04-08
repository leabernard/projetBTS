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
	endSemaphore = new QSemaphore(1);
	conveyor = new Conveyor();
	elevator = new Elevator();
	termination = false;
}

ManageConvoy::~ManageConvoy()
{
	delete conveyor;
	delete elevator;
	delete endSemaphore;
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
		connect(conveyor->getETZ512(), SIGNAL(onReadMultipleHoldingRegistersSentence(quint16, QVector<quint16>)), this, SLOT(receiveSensorsValues(quint16, QVector<quint16>)));
		//connect(app, SIGNAL(aboutToQuit()), this, SLOT(disconnectHost()));
		connect(conveyor->getETZ512(), SIGNAL(disconnected()), this, SLOT(onConveyorDisconnected()));
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
//VOIR AVEC GREMONT POUR LA BOUCLE INFINIE DANS sendElevator() QUI EMPECHE LA RECUPERATION DES VALEURS
int ManageConvoy::checkWeight(float weight, int checkoutNum)
{
	QVector<float> weightValues = AllValuesSingleton::getInstance()->getWeightSensors();
	
	if (checkoutNum == 1) {
		weightValues[0]+= weight;
	}
	else if (checkoutNum == 2) {
		weightValues[1] += weight;
	}
	else if (checkoutNum == 3) {
		weightValues[2] += weight;
	}
	
	if (weightValues[0] > 2.04) {
		int result = sendElevator(1);
		return result;
	}
	else if (weightValues[1] > 2.04) {
		int result = sendElevator(2);
		return result;
	}
	else if (weightValues[2] > 2.04) {
		int result = sendElevator(3);
		return result;
	}
	AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
	return 0;
}

int ManageConvoy::checkLength(float length, int checkoutNum)
{
	QVector<float> lenghtValues = AllValuesSingleton::getInstance()->getLengthSensors();

	if (checkoutNum == 1) {
		lenghtValues[0] += length;
	}
	else if (checkoutNum == 2) {
		lenghtValues[1] += length;
	}
	else if (checkoutNum == 3) {
		lenghtValues[2] += length;
	}

	if (lenghtValues[0] > 80.00) {
		lenghtValues[0] = 0.00;
		int result = sendElevator(1);
		return result;
	}
	else if (lenghtValues[1] > 80.00) {
		lenghtValues[1] = 0.00;
		int result = sendElevator(2);
		return result;
	}
	else if (lenghtValues[2] > 80.00) {
		lenghtValues[2] = 0.00;
		int result = sendElevator(3);
		return result;
	}
	AllValuesSingleton::getInstance()->setLengthSensors(lenghtValues[0], lenghtValues[1], lenghtValues[2]);
	return 0;
}

int ManageConvoy::sendElevator(int checkoutNum)
{
	QVector<bool> elevatorState = AllValuesSingleton::getInstance()->getElevatorButton();

	if (!elevatorState[0] && checkoutNum == 1) {
		conveyor->stopConveyor();
		elevator->getArduino()->sendElevator(checkoutNum);
		return 1;
	}
	if (!elevatorState[1] && checkoutNum == 2) {
		conveyor->stopConveyor();
		elevator->getArduino()->sendElevator(checkoutNum);
		return 2;
	}
	if (!elevatorState[2] && checkoutNum == 3) {
		conveyor->stopConveyor();
		elevator->getArduino()->sendElevator(checkoutNum);
		return 3;
	}
		
	
}

int ManageConvoy::checkAlone(bool alone, int checkoutNum)
{
	int result;
	if (checkoutNum == 1 && alone) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 2) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 3) {
		result = sendElevator(checkoutNum);
		return result;
	}
	return result;
}

void ManageConvoy::stateSensors()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		conveyor->stateSensors();
	}
}

void ManageConvoy::display()
{
}

Conveyor * ManageConvoy::getConveyor()
{
	return conveyor;
}

Elevator * ManageConvoy::getElevator()
{
	return elevator;
}



void ManageConvoy::onConveyorDisconnected()
{
	if (termination)
	{
		releaseTerminationLock();
	}
}

void ManageConvoy::disconnectHost()
{
	qDebug() << "Disconnected from host";
	getTerminationLock();
	termination = true;
}

void ManageConvoy::receiveSensorsValues(quint16 address, QVector<quint16> values) {
	bool value1;
	bool value2;
	bool value3;
	if (address == conveyor->getSensorScanner())
	{
				
			//qDebug() << "Capteur 1 : " + QString::number(values[1]);
			value1 = values[1];
		
		
			//qDebug() << "Capteur 2 : " + QString::number(values[2]);
			value2 = values[2];
		
		
			//qDebug() << "Capteur Scanner : " + QString::number(values[0]);
			value3 = values[0];
		
	}
	instance = AllValuesSingleton::getInstance();
	instance->setSensors(value1, value2, value3);
}

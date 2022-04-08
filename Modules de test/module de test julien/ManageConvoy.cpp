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

ManageConvoy::ManageConvoy(QCoreApplication * app, QObject *parent)
	: QObject(parent)
{
	this->app = app;
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

Conveyor * ManageConvoy::getConveyor()
{
	return conveyor;
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
	float value1;
	float value2;
	float value3;
	if (address == conveyor->getSensorScanner())
	{
				
			qDebug() << "Capteur 1 : " + QString::number(values[1]);
			value1 = values[1];
		
		
			qDebug() << "Capteur 2 : " + QString::number(values[2]);
			value2 = values[2];
		
		
			qDebug() << "Capteur Scanner : " + QString::number(values[0]);
			value3 = values[0];
		
	}
	instance = AllValuesSingleton::getInstance();
	instance->setSensors(value1, value2, value3);
}

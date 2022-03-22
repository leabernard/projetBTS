//*********************************************************************************************
//* Programme : Conveyor.cpp								Date : 18/03/2022
//*														Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Manage the conveyor and the sensors.
//*
//*********************************************************************************************
#include <QtCore/QCoreApplication>
#include "Conveyor.h"
#include <qdebug.h>
#include<Windows.h>

Conveyor::Conveyor(QObject *parent)
	: QObject(parent)
{
	//Initialization of the address of the modbus components 
	conveyorAddress = 0000;
}

Conveyor::~Conveyor()
{

}
//Method to start the conveyor
void Conveyor::startConveyor()
{
	//Method to write a word through TCP
	ETZ512->writeSingleWordFC6(conveyorAddress, 0000);
	qDebug() << "tapis ON";
}
//Method to stop de conveyor
void Conveyor::stopConveyor()
{
	//Method to write a word through TCP
	ETZ512->writeSingleWordFC6(conveyorAddress, 0001);
	qDebug() << "tapis OFF";
}
//Method to ask for all sensors values
void Conveyor::stateSensors()
{
}
//Method to connect to the ETZ card via TCP. Reten true if the connection is successful and else if not. 
bool Conveyor::connectToModbus()
{
	QString test = "192.168.64.200";
	ETZ512 = new QModbusTcpClient("192.168.64.200", 502);
	//Method to conect to the ETZ512 card
	ETZ512->connectToHost();
	if (ETZ512->waitForConnected(3000))
	{
		return true;
	}
	else {
		return false;
	}
}
//Metod to return the ETZ512 card 
QModbusTcpClient * Conveyor::getETZ512()
{
	return ETZ512;
}

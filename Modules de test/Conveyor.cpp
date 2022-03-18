//*********************************************************************************************
//* Programme : Conveyor.cpp								Date : 18/03/2022
//*														Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet d'utiliser le convoyeur et de récupérer les capteurs photoélectrique des vérins et du scanner.
//*
//*********************************************************************************************
#include "Conveyor.h"
#include <Windows.h>
#include <stdlib.h> 

Conveyor::Conveyor(QObject *parent)
	: QObject(parent)
{
	//Initialization of the address of the modbus components 
	conveyor = 0000;
}

Conveyor::~Conveyor()
{
	
}
//Method to start the conveyor
void Conveyor::startConveyor()
{
	ETZ512->writeSingleWordFC6(conveyor, 0000);
	qDebug() << "tapis ON";
}
//Method to stop de conveyor
void Conveyor::stopConveyor()
{
	ETZ512->writeSingleWordFC6(conveyor, 0001);
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
	ETZ512 = new QModbusTcpClient(test, 502);
	ETZ512->connectToHost();
	if (ETZ512->waitForConnected(3000))
	{
		qDebug() << "connected";
		return true;
	}
	else {
		return false;
	}
}
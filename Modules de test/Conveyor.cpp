#include "Conveyor.h"
#include <Windows.h>
#include <stdlib.h> 

Conveyor::Conveyor(QObject *parent)
	: QObject(parent)
{
	conveyor = 0000;
}

Conveyor::~Conveyor()
{
	
}

void Conveyor::startConveyor()
{
	ETZ512->writeSingleWordFC6(conveyor, 0000);
	qDebug() << "tapis ON";
}

void Conveyor::stopConveyor()
{
	ETZ512->writeSingleWordFC6(conveyor, 0001);
	qDebug() << "tapis OFF";
}

void Conveyor::stateSensors()
{
}

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
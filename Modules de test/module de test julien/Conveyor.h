//*********************************************************************************************
//* Programme : Conveyor.h								Date : 18/03/2022
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
#pragma once

#include <QObject>
#include "qmodbustcpclient.h"

class Conveyor : public QObject
{
	Q_OBJECT

private:
	quint16 conveyorAddress;
	quint16 sensor1;
	quint16 sensor2;
	quint16 sensorScanner;
	QModbusTcpClient * ETZ512;

public:
	Conveyor(QObject *parent = Q_NULLPTR);
	~Conveyor();
	void startConveyor();
	void stopConveyor();
	void stateSensors();
	bool connectToModbus();
	QModbusTcpClient * getETZ512();
	quint16 getSensor1();
	quint16 getSensor2();
	quint16 getSensorScanner();
};

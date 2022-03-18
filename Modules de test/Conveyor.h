//*********************************************************************************************
//* Programme : Conveyor.h								Date : 18/03/2022
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
#pragma once

#include <QObject>
#include "qmodbustcpclient.h"

class Conveyor : public QObject
{
	Q_OBJECT

private:
	quint16 conveyor;
	QByteArray sensor1;
	QByteArray sensor2;
	QByteArray sensorScaner;
	QModbusTcpClient * ETZ512;

public:
	Conveyor(QObject *parent = Q_NULLPTR);
	~Conveyor();
	void startConveyor();
	void stopConveyor();
	void stateSensors();
	bool connectToModbus();
};

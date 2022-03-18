//*********************************************************************************************
//* Programme : main.cpp								Date : 18/03/2022
//*														Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					LARIDANT Julien
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT :  Faire communiquer le scanner et les composants du système TAPIRIS.
//*
//*********************************************************************************************
#include <QtCore/QCoreApplication>
#include "Conveyor.h"
#include <Windows.h>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	Conveyor * conveyor = new Conveyor();
	//if we're not coneted we stop the program
	if (conveyor->connectToModbus() == false) {
		qDebug() << "Not connected";
		return a.exec();
	}
	//Method to start and stop the conveyor 
	conveyor->startConveyor();
	Sleep(8000);
	conveyor->stopConveyor();

    return a.exec();
}

//*********************************************************************************************
//* Programme : main.cpp								Date : 18/03/2022
//*														Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					LARIDANT Julien
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT :  Use the classe ManageConvoy.
//*
//*********************************************************************************************
#include <QtCore/QCoreApplication>
#include "ManageConvoy.h"
#include <qdebug.h>
#include<Windows.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	ManageConvoy * manager = new ManageConvoy();
	//Conect to ETZ512 card
	manager->connectToHost();
	//Start conveyor
	//manager->startConveyor();
	//Sleep(1000);
	//Stop conveyor
	//manager->stopConveyor();
	manager->stateSensors();

    return a.exec();
}

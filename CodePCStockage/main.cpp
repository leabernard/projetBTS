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
#include "MonMagnifiqueThread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	
	GlobalThread * thread = new GlobalThread(&a);
	thread->start();
	

    return a.exec();
}

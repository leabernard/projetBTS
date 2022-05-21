#pragma once

#include <qsqldatabase>
#include <QtSql>
#include <QSqlQuery>
#include "Reader.h"
#include "Reader.cpp"
#include <QObject>


class DataBase

{
	Q_OBJECT
private: 
	Reader *reader;



public: 

	int AssociateCommand();
	

	




		

	

};




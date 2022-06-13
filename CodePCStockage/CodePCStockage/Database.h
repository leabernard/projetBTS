#pragma once

#include <QObject>
#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <qsqldatabase.h>
#include <QVariant>
#include <qdebug.h>
#include <qdir.h>
#include <qsqlerror.h>
//#include "text_color.h"

class Database : public QObject
{
	Q_OBJECT

private:
	//HANDLE console;
public:
	Database(QObject *parent = Q_NULLPTR);
	~Database();
	QSqlQuery selectDB(QString request);
	int updateDB(QString request);
	int insertDB(QString request);
	void start();
};

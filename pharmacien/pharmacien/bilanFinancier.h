#pragma once
#include <QApplication>
#include <qsqldatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QtDebug>
#include <QtWidgets/QMainWindow>
#include <QDebug>


class pharmacien;


class bilanFinancier : public QObject
{


	Q_OBJECT

		pharmacien * pharmacienUi;


public:
	bilanFinancier(pharmacien * pharmacienUi, QWidget *parent = Q_NULLPTR);
	void finance();

public slots:
	void Finance();


};




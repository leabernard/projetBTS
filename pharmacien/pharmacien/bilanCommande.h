#pragma once
#include <QApplication>
#include <qsqldatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QtDebug>
#include <QtWidgets/QMainWindow>
#include <QDebug>


class pharmacien;


class bilanCommande : public QObject
{

	
	Q_OBJECT

	pharmacien * pharmacienUi;


	public:
		bilanCommande(pharmacien * pharmacienUi, QWidget *parent = Q_NULLPTR);
		void visual();

	public slots:
		void switchFenetre();
	

};


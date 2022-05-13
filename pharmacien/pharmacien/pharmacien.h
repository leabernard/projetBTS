#pragma once
#include <QApplication>
#include <qsqldatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QtDebug>
#include <QtWidgets/QMainWindow>
#include "ui_pharmacien.h"
#include <QDebug>

class bilanCommande;
class bilanFinancier;

class pharmacien : public QMainWindow
{
    Q_OBJECT

public:
    pharmacien(QWidget *parent = Q_NULLPTR);
	QSqlDatabase db;
	void medicament();
	QString Nom;
	QString idCaisse;
	QString idOrdonnance;
	Ui::pharmacienClass * getUI();
	void setBilanCommande(bilanCommande * bilanPtr);
	void setBilanFinancier(bilanFinancier * bilanfins);

private:
    Ui::pharmacienClass ui;
	bilanCommande * bilanPtr;
	bilanFinancier * bilanFins;
	
public slots:
	void patient();
	void commande();
	void switchFenetre();
	void Finance();
};

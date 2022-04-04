#pragma once
#include <QApplication>
#include <qsqldatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QtDebug>
#include <QtWidgets/QMainWindow>
#include "ui_pharmacien.h"
#include <QDebug>

class pharmacien : public QMainWindow
{
    Q_OBJECT

public:
    pharmacien(QWidget *parent = Q_NULLPTR);
	QSqlDatabase db;
	void medicament();
	QString Nom;

private:
    Ui::pharmacienClass ui;

public slots:
	void patient();
	void commande();
};

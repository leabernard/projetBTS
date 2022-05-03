#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Moduledetestcapteurs.h"
#include "Conveyor.h"
#include "AllValuesSingleton.h"
#include <qcoreapplication.h>
#include <qmutex.h>
#include <qsemaphore.h>
#include "Arduino.h"

class Moduledetestcapteurs : public QMainWindow
{
    Q_OBJECT

public:
    Moduledetestcapteurs(QWidget *parent = Q_NULLPTR);
	void stateSensors();
	


private:
    Ui::ModuledetestcapteursClass ui;
	Conveyor * conveyor;
	AllValuesSingleton * instance;
	Arduino * arduino;

public slots:
	void connectToHost();
	void receiveSensorsValues(quint16 address, QVector<quint16> values);
	void display();
};

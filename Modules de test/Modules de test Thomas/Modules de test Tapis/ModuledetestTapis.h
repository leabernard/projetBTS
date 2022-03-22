#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ModuledetestTapis.h"
#include "Conveyor.h"

class ModuledetestTapis : public QMainWindow
{
    Q_OBJECT

public:
    ModuledetestTapis(QWidget *parent = Q_NULLPTR);
	~ModuledetestTapis();
	void pushCylinder(int checkoutNum);
	void releaseCylinder(int checkoutNum);
	void checkWeight(int weight);
	void checkLength(int length);
	void sendElevator(int checkoutNum);
	void checkAlone(bool alone);
	void stateSensors();
	void stateWeightSensors();
	void stateElevator();
	void display();

private:
    Ui::ModuledetestTapisClass ui;
	Conveyor * conveyor;
	int checkoutNum;
	QList<bool> sensorsDetectionArray;

public slots:
	void startConveyor();
	void stopConveyor();
	void connectToHost();
};

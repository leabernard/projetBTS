#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ModuledetestCylinder.h"
#include "Cylinder.h"
#include "Conveyor.h"

class ModuledetestCylinder : public QMainWindow
{
    Q_OBJECT

public:
    ModuledetestCylinder(QWidget *parent = Q_NULLPTR);
	

private:
    Ui::ModuledetestCylinderClass ui;
	Cylinder * cylinders;
	Conveyor * conveyor;

public slots:
	void connectToHost();
	void pushCylinder1();
	void releaseCylinder1();
	void pushCylinder2();
	void releaseCylinder2();
};

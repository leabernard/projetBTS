#include "ModuledetestCylinder.h"

ModuledetestCylinder::ModuledetestCylinder(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	conveyor = new Conveyor();
}

void ModuledetestCylinder::pushCylinder1()
{
	cylinders->pushCylinder(1);
}

void ModuledetestCylinder::releaseCylinder1()
{
	cylinders->releaseCylinder(1);
}

void ModuledetestCylinder::pushCylinder2()
{
	cylinders->pushCylinder(2);
}

void ModuledetestCylinder::releaseCylinder2()
{
	cylinders->releaseCylinder(2);
}

void ModuledetestCylinder::connectToHost() {
	if (conveyor->connectToModbus() == true) {
		cylinders = new Cylinder(conveyor->getETZ512());
		qDebug() << "Connected";
	}
	else {
		qDebug() << "Not connected";
	}
}
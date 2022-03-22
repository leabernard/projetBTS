#include "ModuledetestTapis.h"

ModuledetestTapis::ModuledetestTapis(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	conveyor = new Conveyor();
}

ModuledetestTapis::~ModuledetestTapis()
{
}
//Method to start the conveyor and write the change in console log
void ModuledetestTapis::startConveyor()
{
	//if we are connected we can continue
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		//Method to start the conveyor 
		conveyor->startConveyor();
	}
}
//Method to stop the conveyor and write the change in console log
void ModuledetestTapis::stopConveyor()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		//Method to stop the conveyor 
		conveyor->stopConveyor();
	}
}
//Method to connect to the ETZ512 card
void ModuledetestTapis::connectToHost()
{
	if (conveyor->connectToModbus() == true) {
		qDebug() << "Connected";
	}
	else {
		qDebug() << "Not connected";
	}
}

void ModuledetestTapis::pushCylinder(int checkoutNum)
{
}

void ModuledetestTapis::releaseCylinder(int checkoutNum)
{
}

void ModuledetestTapis::checkWeight(int weight)
{
}

void ModuledetestTapis::checkLength(int length)
{
}

void ModuledetestTapis::stateSensors()
{
}

void ModuledetestTapis::stateWeightSensors()
{
}

void ModuledetestTapis::stateElevator()
{
}

void ModuledetestTapis::display()
{
}

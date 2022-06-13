//*********************************************************************************************
//* Programme : ManageConvoy.cpp								Date : 18/03/2022
//*																Dernière MAJ : 
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					
//*					 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Manage all sub-classes.
//*
//*********************************************************************************************
#include "ManageConvoy.h"

ManageConvoy::ManageConvoy(QObject *parent)
	: QObject(parent)
{
	endSemaphore = new QSemaphore(1);
	conveyor = new Conveyor();
	arduino = new Arduino();
	termination = false;
}

ManageConvoy::~ManageConvoy()
{
	delete conveyor;
	delete endSemaphore;
}
//Method to start the conveyor
void ManageConvoy::startConveyor()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		conveyor->startConveyor();
	}
}
//Method to stop the conveyor
void ManageConvoy::stopConveyor()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		conveyor->stopConveyor();
	}
}
//Method to connect to the ETZ512 card. If conection is succesful connect some slots to signals and instantiate the cylinders
void ManageConvoy::connectToHost()
{
	if (conveyor->connectToModbus() == true) {
		connect(conveyor->getETZ512(), SIGNAL(onReadMultipleHoldingRegistersSentence(quint16, QVector<quint16>)), this, SLOT(receiveSensorsValues(quint16, QVector<quint16>)));
		connect(conveyor->getETZ512(), SIGNAL(disconnected()), this, SLOT(onConveyorDisconnected()));
		cylinders = new Cylinder(conveyor->getETZ512());
	}
}
//Method to push a cylinder
void ManageConvoy::pushCylinder(int checkoutNum)
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		cylinders->pushCylinder(checkoutNum);
	}
}
//Method to release a cylinder
void ManageConvoy::releaseCylinder(int checkoutNum)
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		cylinders->releaseCylinder(checkoutNum);
	}
}
//Method to check to weight of an elevator and compare it with its own value plus the weight of the next medicine. If the weight is ahead of the maximum accepted, we send the elevator and return the number of the elevator. Else the new value is recorded and 0 is returned
int ManageConvoy::checkWeight(float weight, int checkoutNum)
{
	QVector<float> weightValues = AllValuesSingleton::getInstance()->getWeightSensors();

	if (checkoutNum == 1) {
		weightValues[0] += weight;
	}
	else if (checkoutNum == 2) {
		weightValues[1] += weight;
	}
	else if (checkoutNum == 3) {
		weightValues[2] += weight;
	}

	if (weightValues[0] > 1000) {
		int result = sendElevator(1);
		return result;
	}
	else if (weightValues[1] > 1000) {
		int result = sendElevator(2);
		return result;
	}
	else if (weightValues[2] > 1000) {
		int result = sendElevator(3);
		return result;
	}
	return 0;
}
//Method to check to length of an elevator and compare it with its own value plus the length of the next medicine.
//If the length is ahead of the maximum accepted, we send the elevator and return the number of the elevator. Else the new value is recorded and 0 is returned
int ManageConvoy::checkLength(float length, int checkoutNum)
{
	QVector<float> lenghtValues = AllValuesSingleton::getInstance()->getLengthSensors();

	float lenght1 = 0;
	float lenght2 = 0;
	float lenght3 = 0;
	if (checkoutNum == 1) {
		lenght1 = lenghtValues[0] + length;
	}
	else if (checkoutNum == 2) {
		lenght2 = lenghtValues[1] + length;
	}
	else if (checkoutNum == 3) {
		lenght3 = lenghtValues[2] + length;
	}

	if (lenght1 > 80.00) {
		lenghtValues[0] = 0;
		int result = sendElevator(1);
		AllValuesSingleton::getInstance()->setLengthSensors(lenghtValues[0], lenghtValues[1], lenghtValues[2]);
		return result;
	}
	else if (lenght2 > 80.00) {
		lenghtValues[1] = 0;
		int result = sendElevator(2);
		AllValuesSingleton::getInstance()->setLengthSensors(lenghtValues[0], lenghtValues[1], lenghtValues[2]);
		return result;
	}
	else if (lenght3 > 80.00) {
		lenghtValues[2] = 0;
		int result = sendElevator(3);
		AllValuesSingleton::getInstance()->setLengthSensors(lenghtValues[0], lenghtValues[1], lenghtValues[2]);
		return result;
	}
	
	return 0;
}
//Method to send an elevator. If the elevator isn't already sent, stop the coveyor and send the elevator. Return the number of the elevator sent.
int ManageConvoy::sendElevator(int checkoutNum)
{
	QVector<bool> elevatorState = AllValuesSingleton::getInstance()->getElevatorButton();

	if (!elevatorState[0] && checkoutNum == 1) {
		conveyor->stopConveyor();
		arduino->sendElevator(checkoutNum);
		return 1;
	}
	if (!elevatorState[1] && checkoutNum == 2) {
		conveyor->stopConveyor();
		arduino->sendElevator(checkoutNum);
		return 2;
	}
	if (!elevatorState[2] && checkoutNum == 3) {
		conveyor->stopConveyor();
		arduino->sendElevator(checkoutNum);
		return 3;
	}


}
//Method to check if a medicine needs to be sent alone in an elevator. If it's the case , we send the elevator and return the number of the elevator. Else 0 is returned
int ManageConvoy::checkAlone(bool alone, int checkoutNum)
{
	QVector<float> weightValues = AllValuesSingleton::getInstance()->getWeightSensors();

	int result = 0;
	if (checkoutNum == 1 && alone && weightValues[0] != 0.00) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 1 && alone && weightValues[0] == 0.00) {
		return 4;
	}
	else if (checkoutNum == 2 && alone && weightValues[1] != 0.00) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 2 && alone && weightValues[1] == 0.00) {
		return 5;
	}
	else if (checkoutNum == 3 && alone && weightValues[2] != 0.00) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 3 && alone && weightValues[2] == 0.00) {
		return 6;
	}
	else if (checkoutNum == 1 && !alone) {
		return 7;
	}
	return result;
}
//Method to ask the automate for its sensors
void ManageConvoy::stateSensors()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		conveyor->stateSensors();
	}
}
//Method to display the actual state of the system
void ManageConvoy::display()
{

	console = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coord;
	QVector<bool> conveyorAndCylinders = AllValuesSingleton::getInstance()->getConveyorAndCyliders();
	QVector<bool> sensors = AllValuesSingleton::getInstance()->getSensors();
	QVector<float> weight = AllValuesSingleton::getInstance()->getWeightSensors();
	QVector<float> length = AllValuesSingleton::getInstance()->getLengthSensors();
	QVector<bool> elevatorButton = AllValuesSingleton::getInstance()->getElevatorButton();
	QVector<bool> elevatorState = AllValuesSingleton::getInstance()->getElevatorState();
	float sensor3 = AllValuesSingleton::getInstance()->getSensor3();

	if (conveyorAndCylinders[0] == 1) {
		coord.X = 0;
		coord.Y = 1;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Conveyor : ";
		coord.X = 11;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 0;
		coord.Y = 1;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Conveyor : ";
		coord.X = 11;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (conveyorAndCylinders[1] == 1) {
		coord.X = 25;
		coord.Y = 1;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Cylinder 1 : ";
		coord.X = 38;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 25;
		coord.Y = 1;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Cylinder 1 : ";
		coord.X = 38;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (conveyorAndCylinders[2] == 1) {
		coord.X = 55;
		coord.Y = 1;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Cylinder 2 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 1;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Cylinder 2 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	coord.X = 0;
	coord.Y = 5;
	SetConsoleCursorPosition(console, coord);
	SetConsoleTextAttribute(console, text_color::White);
	qDebug() << "Weight Checkout 1 : ";
	coord.X = 20;
	SetConsoleCursorPosition(console, coord);
	if (weight[0] == 0) {
		qDebug() << "0.00";
	}
	else {
		qDebug() << weight[0];
	}

	coord.X = 0;
	coord.Y = 6;
	SetConsoleCursorPosition(console, coord);
	SetConsoleTextAttribute(console, text_color::White);
	qDebug() << "Weight Checkout 2 : ";
	coord.X = 20;
	SetConsoleCursorPosition(console, coord);
	if (weight[1] == 0) {
		qDebug() << "0.00";
	}
	else {
		qDebug() << weight[1];
	}

	coord.X = 0;
	coord.Y = 7;
	SetConsoleCursorPosition(console, coord);
	SetConsoleTextAttribute(console, text_color::White);
	qDebug() << "Weight Checkout 3 : ";
	coord.X = 20;
	SetConsoleCursorPosition(console, coord);
	if (weight[2] == 0.00) {
		qDebug() << "0.00";
	}
	else {
		qDebug() << weight[2];
	}

	coord.X = 0;
	coord.Y = 9;
	SetConsoleCursorPosition(console, coord);
	SetConsoleTextAttribute(console, text_color::White);
	qDebug() << "Length Checkout 1 : ";
	coord.X = 20;
	SetConsoleCursorPosition(console, coord);
	if (length[0] == 0.00) {
		qDebug() << "0.00";
	}
	else {
		qDebug() << length[0];
	}

	coord.X = 0;
	coord.Y = 10;
	SetConsoleCursorPosition(console, coord);
	SetConsoleTextAttribute(console, text_color::White);
	qDebug() << "Length Checkout 2 : ";
	coord.X = 20;
	SetConsoleCursorPosition(console, coord);
	if (length[1] == 0.00) {
		qDebug() << "0.00";
	}
	else {
		qDebug() << length[1];
	}

	coord.X = 0;
	coord.Y = 11;
	SetConsoleCursorPosition(console, coord);
	SetConsoleTextAttribute(console, text_color::White);
	qDebug() << "Length Checkout 3 : ";
	coord.X = 20;
	SetConsoleCursorPosition(console, coord);
	if (length[2] == 0.00) {
		qDebug() << "0.00";
	}
	else {
		qDebug() << length[2];
	}

	if (elevatorButton[0] == 1) {
		coord.X = 55;
		coord.Y = 5;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Button 1   : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 5;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Button 1   : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (elevatorButton[1] == 1) {
		coord.X = 55;
		coord.Y = 6;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Button 2   : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 6;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Button 2   : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (elevatorButton[2] == 1) {
		coord.X = 55;
		coord.Y = 7;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Button 3   : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 7;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Button 3   : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (elevatorState[0] == 1) {
		coord.X = 55;
		coord.Y = 9;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Elevator 1 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 9;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Elevator 1 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (elevatorState[1] == 1) {
		coord.X = 55;
		coord.Y = 10;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Elevator 2 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 10;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Elevator 2 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}

	if (elevatorState[2] == 1) {
		coord.X = 55;
		coord.Y = 11;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Elevator 3 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 55;
		coord.Y = 11;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Elevator 3 : ";
		coord.X = 68;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";

	}
	if (sensors[0] == 1) {
		coord.X = 0;
		coord.Y = 13;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor 1          : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 0;
		coord.Y = 13;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor 1          : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}
	if (sensors[1] == 1) {
		coord.X = 0;
		coord.Y = 14;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor 2          : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 0;
		coord.Y = 14;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor 2          : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}
	if (sensors[2] == 1) {
		coord.X = 0;
		coord.Y = 16;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor scanner    : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 0;
		coord.Y = 16;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor scanner    : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}
	if (sensor3 == 1) {
		coord.X = 0;
		coord.Y = 15;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor 3          : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Green);
		qDebug() << "true ";
	}
	else {
		coord.X = 0;
		coord.Y = 15;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::White);
		qDebug() << "Sensor 3          : ";
		coord.X = 20;
		SetConsoleCursorPosition(console, coord);
		SetConsoleTextAttribute(console, text_color::Red);
		qDebug() << "false";
	}
}
//Method to get the instance of the conveyor class
Conveyor * ManageConvoy::getConveyor()
{
	return conveyor;
}
//Method to get the instace of the elevator class
Arduino * ManageConvoy::getArduino()
{
	return arduino;
}



void ManageConvoy::onConveyorDisconnected()
{
	if (termination)
	{
		releaseTerminationLock();
	}
}

void ManageConvoy::disconnectHost()
{
	qDebug() << "Disconnected from host";
	getTerminationLock();
	termination = true;
}

void ManageConvoy::receiveSensorsValues(quint16 address, QVector<quint16> values) {
	bool value1 = false;
	bool value2 = false;
	bool value3 = false;
	if (address == conveyor->getSensorScanner())
	{
		value1 = values[1];

		value2 = values[2];

		value3 = values[0];
	}
	instance = AllValuesSingleton::getInstance();
	instance->setSensors(value1, value2, value3);
}

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

	if (weightValues[0] > 2.04) {
		int result = sendElevator(1);
		return result;
	}
	else if (weightValues[1] > 2.04) {
		int result = sendElevator(2);
		return result;
	}
	else if (weightValues[2] > 2.04) {
		int result = sendElevator(3);
		return result;
	}
	AllValuesSingleton::getInstance()->setWeightSensors(weightValues[0], weightValues[1], weightValues[2]);
	return 0;
}
//Method to check to length of an elevator and compare it with its own value plus the length of the next medicine.
//If the length is ahead of the maximum accepted, we send the elevator and return the number of the elevator. Else the new value is recorded and 0 is returned
int ManageConvoy::checkLength(float length, int checkoutNum)
{
	QVector<float> lenghtValues = AllValuesSingleton::getInstance()->getLengthSensors();

	if (checkoutNum == 1) {
		lenghtValues[0] += length;
	}
	else if (checkoutNum == 2) {
		lenghtValues[1] += length;
	}
	else if (checkoutNum == 3) {
		lenghtValues[2] += length;
	}

	if (lenghtValues[0] > 80.00) {
		lenghtValues[0] = 0.00;
		int result = sendElevator(1);
		return result;
	}
	else if (lenghtValues[1] > 80.00) {
		lenghtValues[1] = 0.00;
		int result = sendElevator(2);
		return result;
	}
	else if (lenghtValues[2] > 80.00) {
		lenghtValues[2] = 0.00;
		int result = sendElevator(3);
		return result;
	}
	AllValuesSingleton::getInstance()->setLengthSensors(lenghtValues[0], lenghtValues[1], lenghtValues[2]);
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
	int result;
	if (checkoutNum == 1 && alone) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 2) {
		result = sendElevator(checkoutNum);
		return result;
	}
	else if (checkoutNum == 3) {
		result = sendElevator(checkoutNum);
		return result;
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

	QString conveyor = QString::number(conveyorAndCylinders[0]);
	QString cylinder1 = QString::number(conveyorAndCylinders[1]);
	QString cylinder2 = QString::number(conveyorAndCylinders[2]);

	QString sensor1 = QString::number(sensors[0]);
	QString sensor2 = QString::number(sensors[1]);
	QString sensor3 = QString::number(sensors[2]);

	QString weight1 = QString::number(weight[0]);
	QString weight2 = QString::number(weight[1]);
	QString weight3 = QString::number(weight[2]);

	QString length1 = QString::number(length[0]);
	QString length2 = QString::number(length[1]);
	QString length3 = QString::number(length[2]);

	QString elevatorbutton1 = QString::number(elevatorButton[0]);
	QString elevatorbutton2 = QString::number(elevatorButton[1]);
	QString elevatorbutton3 = QString::number(elevatorButton[2]);

	QString elevatorstate1 = QString::number(elevatorState[0]);
	QString elevatorstate2 = QString::number(elevatorState[1]);
	QString elevatorstate3 = QString::number(elevatorState[2]);

	if (conveyor == '1') {
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

	if (cylinder1 == '1') {
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

	if (cylinder2 == '1') {
		coord.X = 55;
		coord.Y = 0;
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

	if (elevatorbutton1 == '1') {
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

	if (elevatorbutton2 == '1') {
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

	if (elevatorbutton3 == '1') {
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

	if (elevatorstate1 == '1') {
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

	if (elevatorstate2 == '1') {
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

	if (elevatorstate3 == '1') {
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
	bool value1;
	bool value2;
	bool value3;
	if (address == conveyor->getSensorScanner())
	{
		value1 = values[1];

		value2 = values[2];

		value3 = values[0];
	}
	instance = AllValuesSingleton::getInstance();
	instance->setSensors(value1, value2, value3);
}

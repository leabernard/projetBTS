#include "Arduino.h"

Arduino::Arduino(QObject *parent)
	: QObject(parent)
{
	arduinoServer = new QTcpServer();

	if (!arduinoServer->listen(QHostAddress::Any, 502)) {
		qDebug() << "Server not opened";
	}
	connect(arduinoServer, SIGNAL(newConnection()), this, SLOT(arduinoConnection()));
}

Arduino::~Arduino()
{
}
//
QTcpSocket * Arduino::getArduinoSocket()
{
	return arduinoSocket;
}
//Method to send the checkout number to the arduino 
void Arduino::sendElevator(int checkoutNum)
{
	arduinoSocket->write(QByteArray::number(checkoutNum));
}
//Method to receive and interpret the response of the arduino and record the values of the sensors conected to the arduino
void Arduino::receivedData()
{
	QString receivedData = arduinoSocket->read(arduinoSocket->bytesAvailable());
	QStringList splitedData = receivedData.split(";");
	bool button1 = splitedData.at(0).toInt();
	bool button2 = splitedData.at(1).toInt();
	bool button3 = splitedData.at(2).toInt();

	bool elevatorState1 = splitedData.at(3).toInt();
	bool elevatorState2 = splitedData.at(4).toInt();
	bool elevatorState3 = splitedData.at(5).toInt();
	
	QString sensor3result = splitedData.at(6);
	sensor3result = sensor3result.remove("\n").toInt();

	bool stateSensor3 = false;
	if (sensor3result <= 10) {
		stateSensor3 = true;
	}
	else {
		stateSensor3 = false;
	}

	instance = AllValuesSingleton::getInstance();
	instance->setElevatorButton(button1, button2, button3);
	instance->setElevatorState(elevatorState1, elevatorState2, elevatorState3);
	instance->setSensor3(stateSensor3);
}

void Arduino::arduinoDisconnected()
{
	//Demander a gremont ne rentre pas dans le disconnect 
	arduinoSocket->deleteLater();
}
//Method to retrieve the conection and conect slots to signal
void Arduino::arduinoConnection() {
	arduinoSocket = arduinoServer->nextPendingConnection();
	connect(arduinoSocket, SIGNAL(readyRead()), this, SLOT(receivedData()));
	connect(arduinoSocket, SIGNAL(disconnected()), this, SLOT(arduinoDisconnected()));
}

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

QTcpSocket * Arduino::getArduinoSocket()
{
	return arduinoSocket;
}

void Arduino::sendElevator(int checkoutNum)
{
	qDebug() << "Send " << checkoutNum << " to arduino";
	arduinoSocket->write(QByteArray::number(checkoutNum));
}

void Arduino::receivedData()
{
	QString receivedData = arduinoSocket->read(arduinoSocket->bytesAvailable());
	QStringList splitedData = receivedData.split(";");
	bool button1 = splitedData.at(0).toInt();
	bool button2 = splitedData.at(1).toInt();
	bool button3 = splitedData.at(2).toInt();

	float weight1 = splitedData.at(3).toFloat();
	float weight2 = splitedData.at(4).toFloat();
	float weight3 = splitedData.at(5).toFloat();

	bool elevatorState1 = splitedData.at(6).toInt();
	bool elevatorState2 = splitedData.at(7).toInt();
	bool elevatorState3 = splitedData.at(8).toInt();
	qDebug() << elevatorState1 << elevatorState2 << elevatorState3;

	instance = AllValuesSingleton::getInstance();
	instance->setWeightSensors(weight1, weight2, weight3);
	instance->setElevatorButton(button1, button2, button3);
	instance->setElevatorState(elevatorState1, elevatorState2, elevatorState3);
}

void Arduino::arduinoDisconnected()
{
	//Demander a gremont ne rentre pas dans le disconnect 
	arduinoSocket->deleteLater();
	qDebug() << "Arduino deconnectee";
}

void Arduino::arduinoConnection() {
	arduinoSocket = arduinoServer->nextPendingConnection();
	connect(arduinoSocket, SIGNAL(readyRead()), this, SLOT(receivedData()));
	connect(arduinoSocket, SIGNAL(disconnected()), this, SLOT(arduinoDisconnected()));
	qDebug() << "Arduino connectee";
}

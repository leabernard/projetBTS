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

	instance = AllValuesSingleton::getInstance();
	instance->setWeightSensors(weight1, weight2, weight3);
	instance->setElevatorState(button1, button2, button3);
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

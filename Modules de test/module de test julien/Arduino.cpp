#include "Arduino.h"

Arduino::Arduino(QObject *parent)
	: QObject(parent)
{
	connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
	connect(this, SIGNAL(disconnected()), this, SLOT(newDisconnection()));
	connect(this, SIGNAL(readyRead()), this, SLOT(receivedData()));
}

Arduino::~Arduino()
{
}

void Arduino::askData(QByteArray data)
{
}

void Arduino::newConnection() {
	qDebug() << "Arduino connectee";
}

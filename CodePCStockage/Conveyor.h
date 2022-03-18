#pragma once

#include <QObject>
#include "qmodbustcpclient.h"

class Conveyor : public QObject
{
	Q_OBJECT

private:
	quint16 conveyor;
	QByteArray sensor1;
	QByteArray sensor2;
	QByteArray sensorScaner;
	QModbusTcpClient * ETZ512;

public:
	Conveyor(QObject *parent = Q_NULLPTR);
	~Conveyor();
	void startConveyor();
	void stopConveyor();
	void stateSensors();
	bool connectToModbus();
};

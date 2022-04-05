#pragma once

#include <QObject>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include "AllValuesSingleton.h"

class Arduino : public QObject
{
	Q_OBJECT

private:
	QTcpServer * arduinoServer;
	QTcpSocket * arduinoSocket;
	AllValuesSingleton * instance;
public:
	Arduino(QObject *parent = Q_NULLPTR);
	~Arduino();
	QTcpSocket * getArduinoSocket();

public slots:
	void arduinoConnection();
	void receivedData();
	void arduinoDisconnected();
};

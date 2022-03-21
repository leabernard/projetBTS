#pragma once

#include <QObject>
#include <qtcpsocket.h>

class Arduino : public QObject
{
	Q_OBJECT

private:
	QTcpSocket * arduinoSocket;
public:
	Arduino(QObject *parent);
	~Arduino();
	void sendData(QByteArray data);

public slots:
	void receivedMessage();
};

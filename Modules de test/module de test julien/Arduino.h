#pragma once

#include <QObject>
#include <qtcpserver.h>

class Arduino : public QObject
{
	Q_OBJECT

private:
	QTcpServer * arduinoServer;
public:
	Arduino(QObject *parent = Q_NULLPTR);
	~Arduino();
	void askData(QByteArray data);

public slots:
	void newConnection();
};

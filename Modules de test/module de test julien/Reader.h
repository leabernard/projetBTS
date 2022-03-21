#pragma once

#include <QObject>
#include <qserialport.h>

class Reader : public QObject
{
	Q_OBJECT

private:
	QSerialPort * scanner;

public:
	Reader(QObject *parent = Q_NULLPTR);
	~Reader();
public slots:
	void receiveMessage();
};
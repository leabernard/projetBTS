#pragma once

#include <QObject>
#include <qserialport.h>
#include "readerSensor.h"

class Reader : public QObject
{
	Q_OBJECT

private:
	QSerialPort * scanner;
	QString result;

public:
	Reader(QObject *parent = Q_NULLPTR);
	~Reader();
	QString getresult();
public slots:
	void receiveMessage();
};
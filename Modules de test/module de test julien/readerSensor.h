#pragma once
#include <QObject>
#include <qserialport.h>


class readerSensor: public QObject

	
{
	Q_OBJECT
	
private:
	QSerialPort * detectionDrug;


public:
	readerSensor(QObject *parent = Q_NULLPTR);
	~readerSensor();

public slots:
	void detection();

};


#pragma once
#include <QObject>
#include <qserialport.h>


class readerSensor:public QObject

{
	Q_OBJECT


private:
	QSerialPort * detectiondrug;
public:
	readerSensor(QObject *parent = Q_NULLPTR);
	~readerSensor();
	
};

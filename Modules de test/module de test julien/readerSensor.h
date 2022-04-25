#pragma once

#include "Reader.h"
#include "Reader.cpp"
#include <QObject>
#include <qserialport.h>


class readerSensor : public QObject
	
{
Q_OBJECT
	
private:
	void detectionDrug();
	Reader *reader;

public:
	bool sensorDouchette = 1;
	readerSensor();

public slots:

};


#pragma once
#include <QObject>


class readerSensor : public QObject
	
{
Q_OBJECT
	
private:
	void detectionDrug();

public:
	bool sensorDouchette = 1;

public slots:

};


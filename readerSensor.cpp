#include "readerSensor.h"
#include <qdebug.h>



readerSensor::readerSensor(QObject * parent)
		:QObject(parent)

{
	
	if (detectionDrug->canReadLine)
		;
	
}

readerSensor::~readerSensor()
{
}

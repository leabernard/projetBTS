#include "readerSensor.h"
//#include "main.cpp"
#include <qdebug.h>
#include "Reader.cpp"



readerSensor::readerSensor() 
{
	reader = new Reader();
}

void readerSensor::detectionDrug() 
{

	if((sensorDouchette = true) &&  (reader->getresult().size > 0))
		{
			qDebug () << "error";
		}
}


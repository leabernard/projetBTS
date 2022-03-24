#include "readerSensor.h"
#include "Reader.cpp"



readerSensor::readerSensor(QObject * parent)

	 : QObject(parent)
		{
	detectiondrug = new QSerialPort();
	detectiondrug->setPortName("");
	detectiondrug->setBaudRate(9600);
		if (detectiondrug->open(QIODevice::ReadWrite)) {

			connect(detectiondrug, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
		}
}

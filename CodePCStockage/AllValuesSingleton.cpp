#include "AllValuesSingleton.h"

AllValuesSingleton * AllValuesSingleton::instance = nullptr;

AllValuesSingleton::AllValuesSingleton(QObject *parent)
	: QObject(parent)
{
}

AllValuesSingleton::~AllValuesSingleton()
{
}

AllValuesSingleton * AllValuesSingleton::getInstance()
{
	if (instance == nullptr)
		instance = new AllValuesSingleton();
	return instance;
}

void AllValuesSingleton::setSensors(bool sensor1, bool sensor2, bool sensorScanner)
{
	lock.lock();
	this->sensor1 = sensor1;
	this->sensor2 = sensor2;
	this->sensorScanner = sensorScanner;
	lock.unlock();
}

QVector<bool> AllValuesSingleton::getSensors()
{
	lock.lock();
	QVector<bool> result;
	result.push_back(sensor1);
	result.push_back(sensor2);
	result.push_back(sensorScanner);
	lock.unlock();
	return result;
}

void AllValuesSingleton::setWeightSensors(float weight1, float weight2, float weight3)
{
	lock.lock();
	this->weight1 = weight1;
	this->weight2 = weight2;
	this->weight3 = weight3;
	lock.unlock();
}

QVector<float> AllValuesSingleton::getWeightSensors()
{
	return QVector<float>();
}

void AllValuesSingleton::setElevatorState(bool elevator1, bool elevator2, bool elevator3)
{
	lock.lock();
	this->elevator1 = elevator1;
	this->elevator2 = elevator2;
	this->elevator3 = elevator3;
	lock.unlock();
}

QVector<bool> AllValuesSingleton::getElevatorState()
{
	return QVector<bool>();
}

void AllValuesSingleton::setLengthSensors(float length1, float length2, float length3)
{
	lock.lock();
	this->length1 = length1;
	this->length2 = length2;
	this->length3 = length3;
	lock.unlock();
}

QVector<float> AllValuesSingleton::getLengthSensors()
{
	return QVector<float>();
}

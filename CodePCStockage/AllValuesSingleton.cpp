#include "AllValuesSingleton.h"

AllValuesSingleton::AllValuesSingleton(QObject *parent)
	: QObject(parent)
{
}

AllValuesSingleton::~AllValuesSingleton()
{
}

AllValuesSingleton AllValuesSingleton::getInstance()
{
	return AllValuesSingleton();
}

void AllValuesSingleton::setSensors(bool sensor1, bool sensor2, bool sensorScaner)
{
}

QVector<bool> AllValuesSingleton::getSensors()
{
	return QVector<bool>();
}

void AllValuesSingleton::setWeightSensors(float weight1, float weight2, float weight3)
{
}

QVector<float> AllValuesSingleton::getWeightSensors()
{
	return QVector<float>();
}

void AllValuesSingleton::setElevatorState(bool elevator1, bool elevator2, bool elevator3)
{
}

QVector<bool> AllValuesSingleton::getElevatorState()
{
	return QVector<bool>();
}

void AllValuesSingleton::setLengthSensors(float length1, float length2, float length3)
{
}

QVector<float> AllValuesSingleton::getLengthSensors()
{
	return QVector<float>();
}

#include "AllValuesSingleton.h"

AllValuesSingleton * AllValuesSingleton::instance = nullptr;

AllValuesSingleton::AllValuesSingleton(QObject *parent)
	: QObject(parent)
{
}

AllValuesSingleton::~AllValuesSingleton()
{
}
//Method to get the instance of the class
AllValuesSingleton * AllValuesSingleton::getInstance()
{
	if (instance == nullptr)
		instance = new AllValuesSingleton();
	return instance;
}
//Method to set the values of the sensors on the conveyor
void AllValuesSingleton::setSensors(bool sensor1, bool sensor2, bool sensorScanner)
{
	lock.lock();
	this->sensor1 = sensor1;
	this->sensor2 = sensor2;
	this->sensorScanner = sensorScanner;
	lock.unlock();
}
//Method to get the a vector of the sensors on the conveyor
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
//Method to set the values of the weight sensors
void AllValuesSingleton::setWeightSensors(float weight1, float weight2, float weight3)
{
	lock.lock();
	this->weight1 = weight1;
	this->weight2 = weight2;
	this->weight3 = weight3;
	lock.unlock();
}
//Method to get a vector of the weight values
QVector<float> AllValuesSingleton::getWeightSensors()
{
	lock.lock();
	QVector<float> result;
	result.push_back(weight1);
	result.push_back(weight2);
	result.push_back(weight3);
	lock.unlock();
	return result;
}
//Method to set the value of the button of the elevators
void AllValuesSingleton::setElevatorButton(bool elevatorButton1, bool elevatorButton2, bool elevatorButton3)
{
	lock.lock();
	this->elevatorButton1 = elevatorButton1;
	this->elevatorButton2 = elevatorButton2;
	this->elevatorButton3 = elevatorButton3;
	lock.unlock();
}
//Method to get a vector of the state of the elevators
QVector<bool> AllValuesSingleton::getElevatorButton()
{
	lock.lock();
	QVector<bool> result;
	result.push_back(elevatorButton1);
	result.push_back(elevatorButton2);
	result.push_back(elevatorButton3);
	lock.unlock();
	return result;
}
//Method to set the length values
void AllValuesSingleton::setLengthSensors(float length1, float length2, float length3)
{
	lock.lock();
	this->length1 = length1;
	this->length2 = length2;
	this->length3 = length3;
	lock.unlock();
}
//Method to get a vector of the length values
QVector<float> AllValuesSingleton::getLengthSensors()
{
	lock.lock();
	QVector<float> result;
	result.push_back(length1);
	result.push_back(length2);
	result.push_back(length3);
	lock.unlock();
	return result;
}
//Method to set the values of the state of the elevators
void AllValuesSingleton::setElevatorState(bool elevatorState1, bool elevatorState2, bool elevatorState3)
{
	lock.lock();
	this->elevatorState1 = elevatorState1;
	this->elevatorState2 = elevatorState2;
	this->elevatorState3 = elevatorState3;
	lock.unlock();
}
//Method to get a vector of the state of the elevators
QVector<bool> AllValuesSingleton::getElevatorState()
{
	lock.lock();
	QVector<bool> result;
	result.push_back(elevatorState1);
	result.push_back(elevatorState2);
	result.push_back(elevatorState3);
	lock.unlock();
	return result;
}

void AllValuesSingleton::setConveyor(bool conveyorState)
{
	lock.lock();
	this->conveyorState = conveyorState;
	lock.unlock();
}

void AllValuesSingleton::setCylinder1(bool cylinder1)
{
	lock.lock();
	this->cylinder1 = cylinder1;
	lock.unlock();
}

void AllValuesSingleton::setCylinder2(bool cylinder2)
{
	lock.lock();
	this->cylinder2 = cylinder2;
	lock.unlock();
}

QVector<bool> AllValuesSingleton::getConveyorAndCyliders()
{
	lock.lock();
	QVector<bool> result;
	result.push_back(conveyorState);
	result.push_back(cylinder1);
	result.push_back(cylinder2);
	lock.unlock();
	return result;
}

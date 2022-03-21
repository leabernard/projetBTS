#pragma once

#include <QObject>

class AllValuesSingleton : public QObject
{
	Q_OBJECT

private:
	bool sensor1;
	bool sensor2;
	bool sensorScanner;
	float weight1;
	float weight2;
	float weight3;
	float length1;
	float length2;
	float length3;
	bool elevator1;
	bool elevator2;
	bool elevator3;
	static AllValuesSingleton * instance;
	AllValuesSingleton(QObject *parent);
	~AllValuesSingleton();
public:
	static AllValuesSingleton getInstance();
	void setSensors(bool sensor1, bool sensor2, bool sensorScaner);
	QVector<bool> getSensors();
	void setWeightSensors(float weight1, float weight2, float weight3);
	QVector<float> getWeightSensors();
	void setElevatorState(bool elevator1, bool elevator2, bool elevator3);
	QVector<bool> getElevatorState();
	void setLengthSensors(float length1, float length2, float length3);
	QVector<float> getLengthSensors();
};
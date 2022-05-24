#pragma once

#include <QObject>
#include <qmutex.h>
class AllValuesSingleton : public QObject
{
	Q_OBJECT

private:
	bool sensor1 = false;
	bool sensor2 = false;
	bool sensorScanner = false;
	float weight1 = 0.0;
	float weight2 = 0.0;
	float weight3 = 0.0;
	float length1;
	float length2;
	float length3;
	bool elevatorButton1;
	bool elevatorButton2;
	bool elevatorButton3;
	bool elevatorState1;
	bool elevatorState2;
	bool elevatorState3;
	static AllValuesSingleton * instance;
	AllValuesSingleton(QObject *parent = Q_NULLPTR);
	~AllValuesSingleton();

	QMutex lock;

public:
	static AllValuesSingleton * getInstance();
	void setSensors(bool sensor1, bool sensor2, bool sensorScanner);
	QVector<bool> getSensors();
	void setWeightSensors(float weight1, float weight2, float weight3);
	QVector<float> getWeightSensors();
	void setElevatorButton(bool elevatorButton1, bool elevatorButton2, bool elevatorButton3);
	QVector<bool> getElevatorButton();
	void setLengthSensors(float length1, float length2, float length3);
	QVector<float> getLengthSensors();
	void setElevatorState(bool elevatorState1, bool elevatorState2, bool elevatorState3);
	QVector<bool>getElevatorState();
};

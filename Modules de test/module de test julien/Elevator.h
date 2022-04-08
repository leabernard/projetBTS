#pragma once

#include <QObject>

class Elevator : public QObject
{
	Q_OBJECT

public:
	Elevator(QObject *parent = Q_NULLPTR);
	~Elevator();
	void upElevator(int checkoutNum);
	void downElevator(int checkoutNum);
	void stateWeightSensor(int checkoutNum);
	void stateElevator(int checkoutNum);
	void connectToArduino();
};

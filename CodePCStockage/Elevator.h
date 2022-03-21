#pragma once

#include <QObject>

class Elevator : public QObject
{
	Q_OBJECT

public:
	Elevator(QObject *parent);
	~Elevator();
	void upElevator(int checkoutNum);
	void downElevator(int checkoutNum);
	void stateWeightSensor(int checkoutNum);
	void stateElevator(int checkoutNum);
	void connextToHost();
};

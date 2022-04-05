#pragma once

#include <QObject>
#include "Arduino.h"

class Elevator : public QObject
{
	Q_OBJECT

private:
	Arduino * arduino;
public:
	Elevator(QObject *parent = Q_NULLPTR);
	~Elevator();
	void upElevator(int checkoutNum);
	void downElevator(int checkoutNum);
	Arduino * getArduino();
};

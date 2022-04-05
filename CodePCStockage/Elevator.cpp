#include "Elevator.h"

Elevator::Elevator(QObject *parent)
	: QObject(parent)
{
	arduino = new Arduino();
}

Elevator::~Elevator()
{
}

void Elevator::upElevator(int checkoutNum)
{
}

void Elevator::downElevator(int checkoutNum)
{
}

Arduino * Elevator::getArduino()
{
	return arduino;
}



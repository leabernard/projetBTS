#include "Cylinder.h"

Cylinder::Cylinder(QModbusTcpClient * ETZ512, QObject *parent)
	: QObject(parent)
{
	cylinder1 = 0006;
	cylinder2 = 0007;
	this->ETZ512 = ETZ512;
}

Cylinder::~Cylinder()
{
}

void Cylinder::pushCylinder(int checkoutNum)
{
	if (checkoutNum == 1) {
		//Method to write a word through TCP
		ETZ512->writeSingleWordFC6(cylinder1, 0001);
	}
	if (checkoutNum == 2) {
		//Method to write a word through TCP
		ETZ512->writeSingleWordFC6(cylinder2, 0001);
	}
}

void Cylinder::releaseCylinder(int checkoutNum)
{
	if (checkoutNum == 1) {
		//Method to write a word through TCP
		ETZ512->writeSingleWordFC6(cylinder1, 0000);
	}
	if (checkoutNum == 2) {
		//Method to write a word through TCP
		ETZ512->writeSingleWordFC6(cylinder2, 0000);
	}
	
}

QModbusTcpClient * Cylinder::getETZ512()
{
	return ETZ512;
}

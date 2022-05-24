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
//Method to a cylinder
void Cylinder::pushCylinder(int checkoutNum)
{
	if (checkoutNum == 1) {
		ETZ512->writeSingleWordFC6(cylinder1, 0001);
	}
	if (checkoutNum == 2) {
		ETZ512->writeSingleWordFC6(cylinder2, 0001);
	}
}
//Method to release a cylinder
void Cylinder::releaseCylinder(int checkoutNum)
{
	if (checkoutNum == 1) {
		ETZ512->writeSingleWordFC6(cylinder1, 0000);
	}
	if (checkoutNum == 2) {
		ETZ512->writeSingleWordFC6(cylinder2, 0000);
	}
	
}

#include "Cylinder.h"

Cylinder::Cylinder(QObject *parent)
	: QObject(parent)
{
	cylinder1 = 0006;
	cylinder2 = 0007;
}

Cylinder::~Cylinder()
{
}

void Cylinder::pushCylinder(int checkoutNum)
{
	//Method to write a word through TCP
	ETZ512->writeSingleWordFC6(0007, 0001);
	qDebug() << "tapis ON";
}

void Cylinder::releaseCylinder(int checkoutum)
{
}

#pragma once

#include <QObject>
#include "qmodbustcpclient.h"

class Cylinder : public QObject
{
	Q_OBJECT

private:
	quint16 cylinder1;
	quint16 cylinder2;
	QModbusTcpClient * ETZ512;
public:
	Cylinder(QModbusTcpClient * ETZ512, QObject *parent = Q_NULLPTR);
	~Cylinder();
	void pushCylinder(int checkoutNum);
	void releaseCylinder(int checkoutNum);
	QModbusTcpClient * getETZ512();
};

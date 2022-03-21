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
	Cylinder(QObject *parent);
	~Cylinder();
	void pushCylinder(int checkoutNum);
	void releaseCylinder(int checkoutum);
};

#include <QtCore/QCoreApplication>
#include "Conveyor.h"
#include <Windows.h>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	Conveyor * conveyor = new Conveyor();
	if (conveyor->connectToModbus() == false) {
		qDebug() << "Not connected";
		return a.exec();
	}
	conveyor->startConveyor();
	Sleep(8000);
	conveyor->stopConveyor();

    return a.exec();
}

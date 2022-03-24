#include <QtCore/QCoreApplication>
#include "Reader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	Reader * scannerData = new Reader();
    return a.exec();
}

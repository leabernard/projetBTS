#include "TCPServer.h"
#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	TCPServer server = new TCPServer();
    return a.exec();
}

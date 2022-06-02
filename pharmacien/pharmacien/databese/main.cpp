#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database.h"
#include <qdebug.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	database db = new database();
	qDebug() << "a";
	db.start();
	db.insert();
    return a.exec();



}

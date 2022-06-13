#pragma once

#include <QObject>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qjsondocument.h>
#include <qjsonvalue.h>
#include <qjsonobject.h>
#include "Database.h"
#include <qjsonarray.h>
#include <qsqlrecord.h>
#include <qdebug.h>
#include <qsqlfield.h>
#include "ManageConvoy.h"
#include <qtimer.h>

class TCPServer : public QObject
{
	Q_OBJECT

private:
	QTcpServer * server;
	Database * db;
	ManageConvoy * manager;
	QVector<QTcpSocket *> tcpClients;
	QTimer * timer;
public:
	TCPServer(Database * db, ManageConvoy * manager, QObject *parent = Q_NULLPTR);
	~TCPServer();
public slots:
	void onServerNewConnection();
	void onClientNewCommunication();
	void onClientDisonnection();
	void unwantedMedicine();
	void sendSystemState();
	void medicineNotScanned();
};

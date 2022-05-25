#pragma once

#include <QObject>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

class TCPServer : public QObject
{
	Q_OBJECT

private:
	QTcpServer * server;
	QVector<QTcpSocket *> tcpClients;
public:
	TCPServer(QObject *parent = Q_NULLPTR);
	void insertRequests(QByteArray message);
	void selectRequests(QByteArray message);
	~TCPServer();
public slots:
	void onServerNewConnection();
	void onClientNewCommunication();
	void onClientDisonnection();
};

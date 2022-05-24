#pragma once

#include <QObject>
#include <qtcpserver.h>
#include <qtcpsocket.h>

class TCPServer : public QObject
{
	Q_OBJECT

private:
	QTcpServer * server;
	//Database * db;
	QVector<QTcpSocket *> tcpClients;
public:
	TCPServer(Database * db, QObject *parent);
	void insertRequests(QByteArray message);
	void selectRequests(QByteArray message);
	~TCPServer();
public slots:
	void onServerNewConnection();
	void onClientNewCommunication();
	void onClientDisonnection();
};

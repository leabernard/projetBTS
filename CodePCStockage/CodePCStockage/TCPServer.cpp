#include "TCPServer.h"
TCPServer::TCPServer(Database * db, QObject *parent)
	: QObject(parent)
{
	this->db = db;
	server = new QTcpServer(this);
	server->listen(QHostAddress::AnyIPv4, 777);
	connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));
}

void TCPServer::insertRequests(QByteArray message)
{
}

void TCPServer::selectRequests(QByteArray message)
{
}

TCPServer::~TCPServer()
{
}

void TCPServer::onServerNewConnection()
{
	QTcpSocket * socket = server->nextPendingConnection();
	connect(socket, SIGNAL(readyRead()), this, SLOT(onClientNewCommunication()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(onClientDisonnection()));
	tcpClients.append(socket);
}
void TCPServer::onClientNewCommunication()
{
	QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
	QByteArray data = socket->read(socket->bytesAvailable());
	QString message = data.QString::fromStdString(message.toStdString());
	QList<QString> messageSplitedBySemicolon = message.split(",");

}

void TCPServer::onClientDisonnection()
{
	QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(onClientNewCommunication()));
	disconnect(socket, SIGNAL(disconected()), this, SLOT(onClientDisonnection()));
	tcpClients.removeOne(socket);
	socket->deleteLater();
}

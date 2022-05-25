#include "TCPServer.h"

TCPServer::TCPServer(QObject *parent)
	: QObject(parent)
{
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
	//get the socket of the sender and read the message
	QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
	QByteArray data = socket->read(socket->bytesAvailable());

	//Put the message (a json) in a QJsonDocument and get the content of the document
	QJsonDocument jsonMessage = QJsonDocument::fromJson(data);
	QJsonObject jsonContent = jsonMessage.object();

	//get the wanted information thanks to the key name   
	QJsonValue type = jsonContent.value("Type");
	QJsonValue content = jsonContent.value("Content");

	//Show the content obtained
	qDebug() << jsonMessage;
	qDebug() << type.toString();
	qDebug() << content.toString();

	//Prepare a json request
	QJsonDocument documentToSend;
	QJsonObject objectToSend;
	QString jsonToSend;
	objectToSend.insert("Response", "Message Received");
	documentToSend.setObject(objectToSend);

	//Transform the QJsonDocument in json string and send it to the client
	jsonToSend = documentToSend.toJson();
	socket->write(jsonToSend.toUtf8());

}

void TCPServer::onClientDisonnection()
{
	QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(onClientNewCommunication()));
	disconnect(socket, SIGNAL(disconected()), this, SLOT(onClientDisonnection()));
	tcpClients.removeOne(socket);
	socket->deleteLater();
}

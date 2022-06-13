#include "TCPServer.h"
TCPServer::TCPServer(Database * db, ManageConvoy * manager, QObject *parent)
	: QObject(parent)
{
	this->db = db;
	this->manager = manager;
	server = new QTcpServer(this);
	server->listen(QHostAddress::AnyIPv4, 777);
	connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));
//	timer->start(5000);
//	connect(timer, SIGNAL(timeout()), this, SLOT(sendSystemState()));
}

TCPServer::~TCPServer()
{
}

void TCPServer::onServerNewConnection()
{
	QTcpSocket * socket = server->nextPendingConnection();
	qDebug() << "client connecte";
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

	QJsonDocument resultMessage;
	

	if (type == "Request") {
		QStringList parsedContent = content.toString().split(" ");
		if (parsedContent[0] == "SELECT") {
			QSqlQuery responseSelect = db->selectDB(content.toString());
			int responseSelectRowNumber;
			if (responseSelect.last())
			{
				responseSelectRowNumber = responseSelect.at() + 1;
				responseSelect.first();
				responseSelect.previous();
			}
			if (responseSelectRowNumber > 0) {
				QJsonArray arrayForQueryLine;
				while (responseSelect.next()) {
					QJsonObject queryLine;
					
					int i;
					for (i = 0; i < responseSelect.record().count(); i++) {
						queryLine.insert(QString::fromStdString(responseSelect.record().fieldName(i).toStdString()), responseSelect.value(i).toJsonValue());
					}
					arrayForQueryLine.append(queryLine);					
				}
				QJsonObject mesageObject;
				mesageObject.insert("Type", "Request");
				mesageObject.insert("Content", arrayForQueryLine);
				resultMessage.setObject(mesageObject);
				socket->write(resultMessage.toJson());
			}
		}
		else if (parsedContent[0] == "INSERT") {
			QJsonObject object;
			if (db->insertDB(content.toString())) {
				object.insert("Response", "true");
			}
			else {
				object.insert("Response", "false");
			}
			resultMessage.setObject(object);
			socket->write(resultMessage.toJson());
		}
	}
}

void TCPServer::onClientDisonnection()
{
	QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(onClientNewCommunication()));
	disconnect(socket, SIGNAL(disconected()), this, SLOT(onClientDisonnection()));
	tcpClients.removeOne(socket);
	socket->deleteLater();
}

void TCPServer::unwantedMedicine()
{
	QJsonDocument errorMessage;
	QJsonObject object;

	object.insert("Type", "Error");
	object.insert("Content", "Unwanted Medicine got on the conveyor, it will be transported in checkout 1");
	errorMessage.setObject(object);
	for (QTcpSocket * socket : tcpClients)
	{
		socket->write(errorMessage.toJson());
	}
}

void TCPServer::sendSystemState()
{
	QJsonDocument stateMessage;
	QJsonObject object;
	if (manager->getConveyor()->getETZ512()->state() == QAbstractSocket::ConnectedState && manager->getArduino()->getArduinoSocket()->state() == QAbstractSocket::ConnectedState) {
		object.insert("Type", "State");
		object.insert("Content", "OK");
	}
	else {
		object.insert("Type", "State");
		object.insert("Content", "OK");
	}
	stateMessage.setObject(object);
	for (QTcpSocket * socket : tcpClients)
	{
		socket->write(stateMessage.toJson());
	}
}

void TCPServer::medicineNotScanned()
{
	QJsonDocument errorMessage;
	QJsonObject object;

	object.insert("Type", "Error");
	object.insert("Content", "Medicine not scanned got on the conveyor, it will be transported in checkout 1");
	errorMessage.setObject(object);
	for (QTcpSocket * socket : tcpClients)
	{
		socket->write(errorMessage.toJson());
	}
}

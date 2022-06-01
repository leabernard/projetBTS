#include "Moduledetestcapteurs.h"


Moduledetestcapteurs::Moduledetestcapteurs(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	conveyor = new Conveyor();
	arduino = new Arduino();
}

void Moduledetestcapteurs::stateSensors()
{
	if (conveyor->getETZ512()->state() == QTcpSocket::ConnectedState) {
		conveyor->stateSensors();
	}
}

void Moduledetestcapteurs::display()
{
	stateSensors();

	ui.textEdit->clear();

	QVector<bool> sensors = AllValuesSingleton::getInstance()->getSensors();
	QVector<float> weight = AllValuesSingleton::getInstance()->getWeightSensors();
	QVector<float> length = AllValuesSingleton::getInstance()->getLengthSensors();
	QVector<bool> elevatorButton = AllValuesSingleton::getInstance()->getElevatorButton();
	QVector<bool> elevatorState = AllValuesSingleton::getInstance()->getElevatorState();

	QString sensor1 = QString::number(sensors[0]);
	QString sensor2 = QString::number(sensors[1]);
	QString sensor3 = QString::number(sensors[2]);

	QString weight1 = QString::number(weight[0]);
	QString weight2 = QString::number(weight[1]);
	QString weight3 = QString::number(weight[2]);

	QString length1 = QString::number(length[0]);
	QString length2 = QString::number(length[1]);
	QString length3 = QString::number(length[2]);

	QString elevatorbutton1 = QString::number(elevatorButton[0]);
	QString elevatorbutton2 = QString::number(elevatorButton[1]);
	QString elevatorbutton3 = QString::number(elevatorButton[2]);

	QString elevatorstate1 = QString::number(elevatorState[0]);
	QString elevatorstate2 = QString::number(elevatorState[1]);
	QString elevatorstate3 = QString::number(elevatorState[2]);

	ui.textEdit->append(sensor1);
	ui.textEdit->append(sensor2);
	ui.textEdit->append(sensor3);
	ui.textEdit->append(weight1);
	ui.textEdit->append(weight2);
	ui.textEdit->append(weight3);
	ui.textEdit->append(length1);
	ui.textEdit->append(length2);
	ui.textEdit->append(length3);
	ui.textEdit->append(elevatorbutton1);
	ui.textEdit->append(elevatorbutton2);
	ui.textEdit->append(elevatorbutton3);
	ui.textEdit->append(elevatorstate1);
	ui.textEdit->append(elevatorstate2);
	ui.textEdit->append(elevatorstate3);

	qDebug() << weight[0];
	qDebug() << weight[1];
	qDebug() << weight[2];

}

void Moduledetestcapteurs::connectToHost() {
	if (conveyor->connectToModbus() == true) {
		connect(conveyor->getETZ512(), SIGNAL(onReadMultipleHoldingRegistersSentence(quint16, QVector<quint16>)), this, SLOT(receiveSensorsValues(quint16, QVector<quint16>)));
		qDebug() << "Connected";
	}
	else {
		qDebug() << "Not connected";
	}
}

void Moduledetestcapteurs::receiveSensorsValues(quint16 address, QVector<quint16> values)
{
	bool value1;
	bool value2;
	bool value3;
	if (address == conveyor->getSensorScanner())
	{

		//qDebug() << "Capteur 1 : " + QString::number(values[1]);
		value1 = values[1];


		//qDebug() << "Capteur 2 : " + QString::number(values[2]);
		value2 = values[2];


		//qDebug() << "Capteur Scanner : " + QString::number(values[0]);
		value3 = values[0];

	}
	instance = AllValuesSingleton::getInstance();
	instance->setSensors(value1, value2, value3);
}


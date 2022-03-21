#include "Arduino.h"

Arduino::Arduino(QObject *parent)
	: QObject(parent)
{
}

Arduino::~Arduino()
{
}

void Arduino::sendData(QByteArray data)
{
}

void Arduino::receivedMessage() {

}

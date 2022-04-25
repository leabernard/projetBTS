//**********************************************************************************************//
//* Programme	:	reader.cpp																	//
//*																	Date : 			17/03/2022
//*																	Last update : 18/03/2022	//
//*---------------------------------------------------------------------------------------------//
//* Developper	:	julien laridant																//
//*---------------------------------------------------------------------------------------------//
//*																								//
//* 
//*																								//
//**********************************************************************************************//
#include "Reader.h"
#include <qdebug.h>


Reader::Reader(QObject *parent)
	: QObject(parent)
{
	scanner = new QSerialPort();
	scanner->setPortName("COM1");
	scanner->setBaudRate(9600);
	if (scanner->open(QIODevice::ReadWrite)) {
		
		connect(scanner, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	}
}

void Reader::receiveMessage() {
	
	if (scanner->canReadLine())
	{
		QByteArray data = scanner->readLine();
		 result = QString(data).remove("\r").remove("\n");
		
		qDebug() << result;
	}


}   
Reader::~Reader()
{

}
QString Reader::getresult() {
	return result;
}



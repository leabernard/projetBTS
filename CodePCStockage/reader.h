#pragma once

#include <QObject>
#include <qserialport.h>

class Reader : public QObject
{
	Q_OBJECT

private:
	QSerialPort * scanner;
	bool scanned = false;



public:
	Reader(QObject *parent = Q_NULLPTR);
	~Reader();
	bool getScanned();
	void setScanned(bool scanned);
public slots:
	void receiveMessage();
};
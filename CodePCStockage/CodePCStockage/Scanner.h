#pragma once

#include <QObject>
#include <qserialport.h>
#include "Database.h"

class Scanner : public QObject
{
	Q_OBJECT

private:
	QSerialPort * scanner;
	Database * db;
	bool hasScanned;
	QString barCode;
public:
	Scanner(Database * db, QObject *parent = Q_NULLPTR);
	~Scanner();
	QVector<QVariant> identifyMed(QString barCode);
	bool getHasScanned();
	void setHasScanned(bool state);
	QString getBarCode();
public slots:
	void receiveMessage();
	void getNumberOfOrder();
};

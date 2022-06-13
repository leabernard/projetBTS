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
	bool hasScanned = false;
	bool medicineScanned = false;
	QString barCode;
	bool endOfOrder = false;
public:
	Scanner(Database * db, QObject *parent = Q_NULLPTR);
	~Scanner();
	QVector<QVariant> identifyMed();
	bool getHasScanned();
	void setHasScanned(bool state);
	QString getBarCode();
	void incrementDeliveredQuantity(int idOrder, int idPrescription);
	bool orderAvailable();
	bool getEndOfOrder();
	void setEndOfOrder(bool state);
	void setMedicineScanned(bool state);
	bool getMedicineScanned();
public slots:
	void receiveMessage();
};

#pragma once

#include <QObject>

class database : public QObject
{
	Q_OBJECT

public:
	database(QObject *parent =Q_NULLPTR);
	~database();
	void start();
	void insert();
};

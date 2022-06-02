#include "database.h"
#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database.h"
#include <iostream>
#include <qsqldatabase.h>
#include <QVariant>
#include <qdebug.h>
#include <qdir.h>
#include <qsqlerror.h>
database::database(QObject *parent)
	: QObject(parent)
{



}

database::~database()
{
}

void database::start() {
	
	QDir dbPath;
	// QString path = dbPath.currentPath() + "pharmacie.db";
	QString path = "./pharmacie.db";
	qDebug() << path;
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("pharmacie.db");

	qDebug() << "b";
	if (!db.open()) {
		std::cout << "Problème d'ouverture\n";
		qDebug() << "ereur";
	}
	else
	{
		qDebug() << "c";
		QSqlQuery createMedicamentTable;
		QSqlQuery createOrdonnanceTable;
		QSqlQuery createCommandeTable;
		createMedicamentTable.prepare("create table medicament "
			"(idmedicament integer primary key autoincrement, "
			"code_barre int, "
			"longueur int, "
			"hauteur int, "
			"nom varchar(20), "
			"prix_achat_unitaire_ht float, "
			"prix_vente_unitaire_ht float, "
			"individual bool, "
			"largeur integer)");
		qDebug() << createMedicamentTable.exec();
		qDebug() << createMedicamentTable.lastError().text();
		

		createOrdonnanceTable.prepare("create table ordonnance "
			"(idordonnance integer primary key autoincrement, "
			"NomPatient varchar(20), "
			"idcaisse int, "
			"date DATE DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME')))");
		qDebug() << createOrdonnanceTable.exec();
		qDebug() << createOrdonnanceTable.lastError().text();


		createCommandeTable.prepare("create table commande "
			"(idcommande integer primary key autoincrement, "
			"idmedicament integer, "
			"idordonnance integer, "
			"Nom varchar(20), "
			
			"quantite_livrer integer,"
			"FOREIGN KEY(idmedicament) REFERENCES medicament(idmedicament), "
			"FOREIGN KEY(idordonnance) REFERENCES ordonnance(idordonnance)) "
			);
		qDebug() << createCommandeTable.exec();
		qDebug() << createCommandeTable.lastError().text();
	}
}

void database::insert() {
	QSqlQuery query;
	query.prepare("INSERT INTO ordonnance ('idcaisse') VALUES ( :idcaisse)");
	query.bindValue(":idcaisse", 1);
	//query.bindValue(":longueur", 15);
	//query.bindValue(":largeur", 7);
	
	if (query.exec()) {
		qDebug() << "reussi";
		qDebug() << query.lastError().text();
	}
	qDebug() << query.lastError().text();
}
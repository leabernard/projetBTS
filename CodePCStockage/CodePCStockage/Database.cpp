#include "Database.h"
Database::Database(QObject *parent)
	: QObject(parent)
{



}

Database::~Database()
{
}

QSqlQuery Database::selectDB(QString request)
{
	QSqlQuery selectRequest;
	selectRequest.prepare(request);
	selectRequest.exec();
	return selectRequest;
}

void Database::start() {

	//COORD coord;
	QDir dbPath;
	QString path = dbPath.currentPath() + "pharmacie.db";
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	if (QFile::exists(path)) {
		if (!db.open()) {
			/*coord.X = 80;
			coord.Y = 1;
			SetConsoleCursorPosition(console, coord);
			SetConsoleTextAttribute(console, text_color::Red);*/
			qDebug() << db.lastError();
			exit(0);
		}
		else {
			/*coord.X = 80;
			coord.Y = 1;
			SetConsoleCursorPosition(console, coord);
			SetConsoleTextAttribute(console, text_color::Green);*/
			qDebug() << "Database is opened";
		}

		
	}
	else {
		db.setDatabaseName(path);
		if (!db.open()) {
			qDebug() << "Probleme d'ouverture";
		}
		else
		{
			QSqlQuery createMedicamentTable;
			QSqlQuery createOrdonnanceTable;
			QSqlQuery createCommandeTable;
			createMedicamentTable.prepare("create table medicament "
				"(idmedicament integer primary key autoincrement, "
				"code_barre int, "
				"longueur float, "
				"largeur float, "
				"hauteur float, "
				"nom varchar(20), "
				"prix_achat_unitaire_ht float, "
				"prix_vente_unitaire_ht float, "
				"individuel bool, "
				"poids float)");
			qDebug() << createMedicamentTable.exec();
			qDebug() << createMedicamentTable.lastError().text();


			createOrdonnanceTable.prepare("create table ordonnance "
				"(idordonnance integer primary key autoincrement, "
				"nompatient varchar(20), "
				"idcaisse int, "
				"date DATE DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME')))");
			qDebug() << createOrdonnanceTable.exec();
			qDebug() << createOrdonnanceTable.lastError().text();


			createCommandeTable.prepare("create table commande "
				"(idcommande integer primary key autoincrement, "
				"idmedicament integer, "
				"idordonnance integer, "
				"quantite int, "
				"quantite_livree integer, "
				"etatcommande bool, "
				"FOREIGN KEY(idmedicament) REFERENCES medicament(idmedicament), "
				"FOREIGN KEY(idordonnance) REFERENCES ordonnance(idordonnance)) "
			);
			qDebug() << createCommandeTable.exec();
			qDebug() << createCommandeTable.lastError().text();
		}
	}
	
}

#include "pharmacien.h"
#include "bilanCommande.h"
#include"bilanFinancier.h"
#include "ajoutmedicament.h"

pharmacien::pharmacien(QWidget *parent)
	: QMainWindow(parent)
{
	
	
	ui.setupUi(this);
	ui.boutonPatient->setVisible(true);
	ui.image->setVisible(true);
	ui.label->setVisible(true);
	ui.image->setVisible(true);
	ui.labelQuantite->setVisible(true);
	ui.labelStatue->setVisible(true);
	ui.label_2->setVisible(true);
	ui.label_3->setVisible(true);
	ui.lineEditIDCaisse->setVisible(true);
	ui.lineEditNom->setVisible(true);
	ui.listeDeMedoc->setVisible(true);
	ui.quantite->setVisible(true);
	ui.tableCommande->setVisible(false);
	ui.validerCommande->setVisible(true);
	ui.tableFinancier->setVisible(false);
	ui.nom->setVisible(false);
	ui.labelNom->setVisible(false);
	ui.hauteur->setVisible(false);
	ui.labelHauteur->setVisible(false);
	ui.longueur->setVisible(false);
	ui.labelLongueur->setVisible(false);
	ui.codebarre->setVisible(false);
	ui.labelCodebarre->setVisible(false);
	ui.labelprixVente->setVisible(false);
	ui.prixvente->setVisible(false);
	ui.prixachat->setVisible(false);
	ui.labelPrixAchat->setVisible(false);
	ui.individual->setVisible(false);
	ui.labelIndividual->setVisible(false);
	ui.largeur->setVisible(false);
	ui.label_4->setVisible(false);
	ui.ajoutmedoc->setVisible(false);
	ui.poids->setVisible(false);
	ui.labelPoid->setVisible(false);


	//connexion base de donnée
	QSqlDatabase Database;

	QDir dbPath;
	QString path = dbPath.currentPath() + "pharmacie.db";
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);
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
				"Nom integer,"
				"etatcommande bool, "
				"FOREIGN KEY(idmedicament) REFERENCES medicament(idmedicament), "
				"FOREIGN KEY(idordonnance) REFERENCES ordonnance(idordonnance)) "
			);
			qDebug() << createCommandeTable.exec();
			qDebug() << createCommandeTable.lastError().text();
		}
	}

	connect(ui.quantite, SIGNAL(valueChanged(int)), ui.label, SLOT(setNum(int)));

	ui.listeDeMedoc->addItem( "medicament" );
	medicament();
	
	//selction des nom de médoc
}
void pharmacien::medicament() {

	QSqlQuery query( "SELECT nom FROM medicament" );

	while ( query.next() )
		ui.listeDeMedoc->addItem( query.value( 0 ).toString() );

}		//récupération du nom patient
void pharmacien::patient() {
	this->Nom = ui.lineEditNom->text();
	ui.lineEditNom->setText( "" );

	this->idCaisse = ui.lineEditIDCaisse->text();
	ui.lineEditIDCaisse->setText("");

	QString newOrdonnance = "INSERT INTO ordonnance ('idcaisse', 'Nompatient') VALUES ('" + this->idCaisse + "', '" + this->Nom + "')";
	QSqlQuery newOrdonnanceQuery;
	newOrdonnanceQuery.exec(newOrdonnance);

	//crée une nouvelle ordonnace
	QSqlQuery getNewOrdonnance;
	getNewOrdonnance.exec("SELECT idordonnance FROM ordonnance ORDER BY date DESC LIMIT 1");
	if (getNewOrdonnance.next()){
		idOrdonnance = getNewOrdonnance.value(0).toString();
	}
		

}

Ui::pharmacienClass * pharmacien::getUI()
{
	return &ui;
}

void pharmacien::setBilanCommande(bilanCommande * bilanPtr)
{
	this->bilanPtr = bilanPtr;
}

void pharmacien::setBilanFinancier(bilanFinancier * bilanFins)
{
	this->bilanFins = bilanFins;
}

void pharmacien::setajoutmedicament(ajoutmedicament * ajoutmedoc)
{
	this->ajoutmedoc = ajoutmedoc;
}

void pharmacien::commande()
{
	QString IDMedicament;

	//Récupère la nombre de médoc mis  dans le label : 
	QString Quantiter = ui.label->text();
	//On récupère le nom du patient stocker dans la classe

	//On récupère le nom du médoc
	QVariant NomMedoc = ui.listeDeMedoc->itemText(ui.listeDeMedoc->currentIndex());
	//Convertit le QVariant en QString
	QString NomMedocString = NomMedoc.toString();
	//Envoir BDD
	QString SQLIDMedicament = "SELECT `idmedicament` FROM `medicament` WHERE `nom` = '" + NomMedocString +"'";
	QSqlQuery queryIDMedicament;
	queryIDMedicament.exec(SQLIDMedicament);
	if (queryIDMedicament.next()) {
		IDMedicament = queryIDMedicament.value(0).toString();
	}

	//Envoir BDD
	 
	//Insertion de la commande en BDD
	QString SQLcommande = "INSERT INTO commande ( idmedicament,Nom,idordonnance,quantite) VALUES ('"+ IDMedicament +"','"+ this->Nom +"','"+ idOrdonnance +"','"+ Quantiter +"')";
	QSqlQuery querySQLcommande;
	querySQLcommande.exec(SQLcommande);
}

void pharmacien::switchFenetre()
{
	bilanPtr->switchFenetre();
}
void pharmacien::Finance()
{
	bilanFins->Finance();
}
void pharmacien::ajoutMedoc()
{
	ajoutmedoc->ajoutMedoc();
}

void pharmacien::recuperation()
{
	ajoutmedoc->recuperation();
}
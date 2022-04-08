#include "pharmacien.h"

pharmacien::pharmacien(QWidget *parent)
	: QMainWindow(parent)
{

	
	ui.setupUi(this);

	QSqlDatabase Database;

	Database = QSqlDatabase::addDatabase( "QSQLITE" );
	Database.setDatabaseName("./pharmacie.db");

	if ( Database.open( ) ) {
		ui.labelStatue->setText( "Statue de la base de donnee: connecte" );
	}

	connect(ui.quantite, SIGNAL(valueChanged(int)), ui.label, SLOT(setNum(int)));

	ui.listeDeMedoc->addItem( "medicament" );
	medicament();
	
	
}
void pharmacien::medicament() {

	QSqlQuery query( "SELECT nom FROM medicament" );

	while ( query.next() )
		ui.listeDeMedoc->addItem( query.value( 0 ).toString() );

}		
void pharmacien::patient() {
	this->Nom = ui.lineEditNom->text();
	ui.lineEditNom->setText( "" );

	this->idCaisse = ui.lineEditIDCaisse->text();
	ui.lineEditIDCaisse->setText("");

	QString newOrdonnance = "INSERT INTO ordonnance ('idcaisse', 'NomPatient') VALUES ('" + this->idCaisse + "', '" + this->Nom + "')";
	QSqlQuery newOrdonnanceQuery;
	newOrdonnanceQuery.exec(newOrdonnance);


	QSqlQuery getNewOrdonnance;
	getNewOrdonnance.exec("SELECT idordonnance FROM ordonnance ORDER BY date DESC LIMIT 1");
	if (getNewOrdonnance.next()){
		idOrdonnance = getNewOrdonnance.value(0).toString();
	}
		

}

void pharmacien::commande()
{
	QString IDMedicament;

	//R�cup�re la nombre de m�doc mis a la shlag dans le label : 
	QString Quantiter = ui.label->text();
	//On r�cup�re le nom du patient stocker dans la classe

	//On r�cup�re le nom du m�doc
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

	/*Thomas : je sais pad a quoi ca sert.

	QString SQLIDOrdonnance = "SELECT `idordonnance` FROM `ordonnance` WHERE `NomPatient` = '" + this->Nom + "'";
	qDebug() << "SELECT `idordonnance` FROM `ordonnance` WHERE `NomPatient` = '" + this->Nom + "'";
	QSqlQuery queryIDOrdonnance(SQLIDOrdonnance);
	while (queryIDOrdonnance.next())
		qDebug() << "test 6.5";
		IDOrdonnance = queryIDOrdonnance.value(0).toString();
	qDebug() << "test 7";
	*/

	//Insertion de la commande en BDD
	QString SQLcommande = "INSERT INTO commande ( idmedicament,Nom,idordonnance,quantite_livrer) VALUES ('"+ IDMedicament +"','"+ this->Nom +"','"+ idOrdonnance +"','"+ Quantiter +"')";
	QSqlQuery querySQLcommande;
	querySQLcommande.exec(SQLcommande);
}
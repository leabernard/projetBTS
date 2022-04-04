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
}

void pharmacien::commande()
{
	QString IDMedicament;
	QString IDOrdonnance;

	//Récupère la nombre de médoc mis a la shlag dans le label : 
	QString Quantiter = ui.label->text();

	//On récupère le nom du patient stocker dans la classe

	//On récupère le nom du médoc
	QVariant NomMedoc = ui.listeDeMedoc->itemData(ui.listeDeMedoc->currentIndex());
	//Convertit le QVariant en QString
	QString NomMedocString = NomMedoc.toString();

	//Envoir BDD
	QString SQLIDMedicament = "SELECT `idmedicament` FROM `medicament` WHERE `nom` = '" + NomMedocString +"'";
	QSqlQuery queryIDMedicament(SQLIDMedicament);

	while (queryIDMedicament.next())
		IDMedicament = queryIDMedicament.value(0).toString();

	//Envoir BDD
	QString SQLIDOrdonnance = "SELECT `idordonnance` FROM `ordonnance` WHERE `NomPatient` = '" + this->Nom + "'";
	QSqlQuery queryIDOrdonnance(SQLIDOrdonnance);
	while (queryIDOrdonnance.next())
		IDOrdonnance = queryIDOrdonnance.value(0).toString();

	//Insertion de la commande en BDD
	QString SQLcommande = "INSERT INTO commande ( idmedicament,Nom,idordonnance,quantite_livrer) VALUES ('"+ SQLIDMedicament +"','"+ this->Nom +"','"+ SQLIDOrdonnance +"','"+ Quantiter +"')";
}
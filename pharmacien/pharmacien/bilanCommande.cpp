
#include "bilanCommande.h"
#include <QLocale>
#include "pharmacien.h"
#include <iostream>



using namespace std;

bilanCommande::bilanCommande(pharmacien * pharmacienUi, QWidget *parent)
    : QObject(parent)
{
	this->pharmacienUi = pharmacienUi;
}
//visuel de la page
void bilanCommande::switchFenetre()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();
	//qDebug() << "test" ;

	ui->boutonPatient->setVisible(false);
	ui->image->setVisible(false);
	ui->label->setVisible(false);
	ui->image->setVisible(false);
	ui->labelQuantite->setVisible(false);
	ui->labelStatue->setVisible(false);
	ui->label_2->setVisible(false);
	ui->label_3->setVisible(false);
	ui->lineEditIDCaisse->setVisible(false);
	ui->lineEditNom->setVisible(false);
	ui->listeDeMedoc->setVisible(false);
	ui->quantite->setVisible(false);
	ui->tableCommande->setVisible(true);
	ui->validerCommande->setVisible(false);
	ui->nom->setVisible(false);
	ui->labelNom->setVisible(false);
	ui->hauteur->setVisible(false);
	ui->labelHauteur->setVisible(false);
	ui->longueur->setVisible(false);
	ui->labelLongueur->setVisible(false);
	ui->codebarre->setVisible(false);
	ui->labelCodebarre->setVisible(false);
	ui->labelprixVente->setVisible(false);
	ui->prixvente->setVisible(false);
	ui->prixachat->setVisible(false);
	ui->labelPrixAchat->setVisible(false);
	ui->individual->setVisible(false);
	ui->labelIndividual->setVisible(false);
	ui->largeur->setVisible(false);
	ui->label_4->setVisible(false);
	ui->ajoutmedoc->setVisible(false);
	ui->poids->setVisible(false);
	ui->labelPoid->setVisible(false);
	this->visual( );
}//requète pour afficher les commande
void bilanCommande::visual()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();

	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery("select medicament.nom, commande.quantite, commande.idordonnance, commande.idcommande, commande.Nom from medicament, commande where medicament.idmedicament = commande.idmedicament");
	model->setHeaderData(0, Qt::Horizontal, tr("medicament"));
	model->setHeaderData(1, Qt::Horizontal, tr("quantite"));
	model->setHeaderData(2, Qt::Horizontal, tr("ID Ordo"));
	model->setHeaderData(3, Qt::Horizontal, tr("Nom"));
	model->setHeaderData(4, Qt::Horizontal, tr("nom patient"));

	ui->tableCommande->setModel( model );

	
}

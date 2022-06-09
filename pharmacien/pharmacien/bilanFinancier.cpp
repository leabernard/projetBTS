
#include "bilanFinancier.h"
#include "bilanCommande.h"
#include <QLocale>
#include "pharmacien.h"
#include <iostream>



using namespace std;

bilanFinancier::bilanFinancier(pharmacien * pharmacienUi, QWidget *parent)
	: QObject(parent)
{
	this->pharmacienUi = pharmacienUi;
}

void bilanFinancier::Finance()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();
	//qDebug() << "test";

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
	ui->tableCommande->setVisible(false);
	ui->validerCommande->setVisible(false);
	ui->tableFinancier->setVisible(true);
	ui->nom->setVisible(false);
	ui->labelNom->setVisible(false);
	ui->hauteur->setVisible(false);
	ui->labelHauteur->setVisible(false);
	ui->longueur->setVisible(false);
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
	this->finance();
}
void bilanFinancier::finance()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();
	qDebug() << "test2";
	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery("SELECT medicament.nom, (SUM(commande.quantite) * medicament.prix_vente_unitaire_ht) AS '  prix', SUM(commande.quantite) AS 'total_vendu' FROM medicament, commande WHERE commande.idmedicament = medicament.idmedicament GROUP BY commande.idmedicament");
	model->setHeaderData(0, Qt::Horizontal, tr("nom"));
	model->setHeaderData(1, Qt::Horizontal, tr("prix"));
	model->setHeaderData(2, Qt::Horizontal, tr("total_vendu"));
	
	ui->tableFinancier->setModel(model);


}

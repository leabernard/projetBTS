
#include "bilanFinancier.h"
#include "bilanCommande.h"
#include "ajoutmedicament.h"
#include <QLocale>
#include "pharmacien.h"
#include <iostream>



using namespace std;

ajoutmedicament::ajoutmedicament(pharmacien * pharmacienUi, QWidget *parent)
	: QObject(parent)
{
	this->pharmacienUi = pharmacienUi;
}

void ajoutmedicament::ajoutMedoc()
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
	ui->tableFinancier->setVisible(false);
	ui->nom->setVisible(true);
	ui->labelNom->setVisible(true);
	ui->hauteur->setVisible(true);
	ui->labelHauteur->setVisible(true);
	ui->longueur->setVisible(true);
	ui->labelLongueur->setVisible(true);
	ui->codebarre->setVisible(true);
	ui->labelCodebarre->setVisible(true);
	ui->labelprixVente->setVisible(true);
	ui->prixvente->setVisible(true);
	ui->prixachat->setVisible(true);
	ui->labelPrixAchat->setVisible(true);
	ui->individual->setVisible(true);
	ui->labelIndividual->setVisible(true);
	ui->largeur->setVisible(true);
	ui->label_4->setVisible(true);
	ui->ajoutmedoc->setVisible(true);
	ui->poids->setVisible(true);
	ui->labelPoid->setVisible(true);
	

	

	//this->ajoutmedoc();
}

void ajoutmedicament::recuperation()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();

	this->Nom = ui->nom->text();
	ui->nom->setText("");

	this->hauteur = ui->hauteur->text();
	ui->hauteur->setText("");

	this->longeur = ui->longueur->text();
	ui->longueur->setText("");

	this->code_barre = ui->codebarre->text();
	ui->codebarre->setText("");

	this->prix_vente = ui->prixvente->text();
	ui->prixvente->setText("");

	this->prix_achat = ui->prixachat->text();
	ui->prixachat->setText("");

	this->individuel = ui->individual->text();
	ui->individual->setText("");

	this->largeur = ui->largeur->text();
	ui->largeur->setText("");

	this->poids = ui->poids->text();
	ui->poids->setText("");

	QString newMedicament = "INSERT INTO medicament ('code_barre', 'longueur','largeur', 'hauteur','nom', 'prix_achat_unitaire_ht','prix_vente_unitaire_ht', 'individuel', 'poids') VALUES ('" + this->code_barre + "', '" +this->longeur + "', '" + this->largeur + "', '" + this->hauteur + "', '" + this->Nom + "', '" + this->prix_achat + "', '" + this->prix_vente + "', '" + this->individuel + "', '" + this->poids + "')";
	QSqlQuery newMedicamentQuery;
	newMedicamentQuery.exec(newMedicament);
	qDebug() << "test";

}

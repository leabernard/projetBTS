
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

void bilanCommande::switchFenetre()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();
	qDebug() << "test" ;

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
	this->visual( );
}
void bilanCommande::visual()
{
	Ui::pharmacienClass * ui = pharmacienUi->getUI();

	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery("select * from commande");
	model->setHeaderData(0, Qt::Horizontal, tr("ID Commande"));
	model->setHeaderData(1, Qt::Horizontal, tr("ID Med"));
	model->setHeaderData(2, Qt::Horizontal, tr("ID Ordo"));
	model->setHeaderData(3, Qt::Horizontal, tr("Nom"));
	model->setHeaderData(4, Qt::Horizontal, tr("Quantite"));

	ui->tableCommande->setModel( model );
	
}

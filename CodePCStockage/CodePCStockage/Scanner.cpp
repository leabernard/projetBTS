#include "Scanner.h"

Scanner::Scanner(Database * db, QObject *parent)
	: QObject(parent)
{
	this->db = db;
	scanner = new QSerialPort();
	scanner->setPortName("COM1");
	scanner->setBaudRate(9600);
	if (scanner->open(QIODevice::ReadWrite)) {
		connect(scanner, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	}
}

Scanner::~Scanner()
{
}

QVector<QVariant> Scanner::identifyMed()
{
	QVector<QVariant> medInfo;
	QString requestSelectBarcode = "SELECT * FROM Commande, Medicament WHERE Commande.IDMedicament = Medicament.idmedicament AND Medicament.code_barre = " + barCode;
	QSqlQuery responseVerifyingMed = db->selectDB(requestSelectBarcode);
	
	int responseVerifyingMedRowNumber;
	if (responseVerifyingMed.last()) {

		responseVerifyingMedRowNumber = responseVerifyingMed.at() + 1;
		responseVerifyingMed.first();
		responseVerifyingMed.previous();
	}
	if(responseVerifyingMedRowNumber > 0){

		QString requestMedInformation = "SELECT CASE WHEN medicament.longueur > medicament.hauteur AND medicament.longueur > medicament.largeur THEN medicament.longueur  WHEN medicament.hauteur > medicament.largeur THEN medicament.hauteur ELSE medicament.largeur END AS 'taille', ordonnance.idcaisse, medicament.individuel, medicament.poids, commande.idcommande, ordonnance.idordonnance FROM medicament, ordonnance, commande WHERE medicament.idmedicament = commande.idmedicament AND commande.idordonnance = ordonnance.idordonnance AND commande.etatcommande = 0 AND medicament.code_barre = '" + barCode + "' ORDER by ordonnance.date DESC LIMIT 1";
		QSqlQuery responseMedInformation = db->selectDB(requestMedInformation);
		qDebug() << requestMedInformation;
		if (responseMedInformation.next()) {
			medInfo.push_back(responseMedInformation.value(0).toFloat());
			medInfo.push_back(responseMedInformation.value(1).toInt());
			medInfo.push_back(responseMedInformation.value(2).toInt());
			medInfo.push_back(responseMedInformation.value(3).toFloat());
			medInfo.push_back(responseMedInformation.value(4).toInt());
			medInfo.push_back(responseMedInformation.value(5).toInt());
			return medInfo;
		}
		
	}
	medInfo.push_back(-1);
	return medInfo;
	//Utilise une requete qui select les medics des ordonnances en fonction du code barres recu et retourne la réponse brut pour pouvoir fetch le resultat.
	//if(fetch())
	//alors on utilise une requete pour obtenir le numéro de caisse de ce médicament ( sachant qu'on prendra le medicament le plus anciennement commandé )
	//finalement on envoie un signal au thread qui contient le numéro de caisse
	//si il n'y a pas de fetch le medicament est indesirable donc on envoie un signal signifiant un problème


	//Pour changer l'état des commandes en cours faire une methode au moment ou les vérins poussent (caisse 1 et 2) ou quand le poids augmente(caisse 3) qui ejectera d'un tableau préalablement créer le medicaments correspondant au numero de caisse et a l'idée puis enverra une requète pour decrementer le nombre du médicament ejecter sur l'ordonance correspondante
}

bool Scanner::getHasScanned()
{
	return hasScanned;
}

void Scanner::setHasScanned(bool state)
{
	hasScanned = state;
}

QString Scanner::getBarCode()
{
	return barCode;
}

void Scanner::incrementDeliveredQuantity(int idOrder, int idPrescription)
{
	QString requestIncrementQuantity = "UPDATE commande SET quantite_livree = quantite_livree + 1 WHERE idcommande = " + QString::number(idOrder);
	if (db->updateDB(requestIncrementQuantity)) {
		QString requestQuantities = "SELECT quantite, quantite_livree FROM commande WHERE idcommande = " + QString::number(idOrder);
		QSqlQuery responseQuantites = db->selectDB(requestQuantities);
		int responseQuantitesRowNumber = 0;
		if (responseQuantites.last())
		{
			responseQuantitesRowNumber = responseQuantites.at() + 1;
			responseQuantites.first();
			responseQuantites.previous();
		}
		
		if (responseQuantitesRowNumber > 0) {
			responseQuantites.next();
			int quantity = responseQuantites.value(0).toInt();
			int deliveredQuantity = responseQuantites.value(1).toInt();
			if (quantity == deliveredQuantity) {
				QString requestOrderState = "UPDATE commande SET etatcommande = true WHERE idcommande = " + QString::number(idOrder);
				db->updateDB(requestOrderState);
			}
		}
		int rowFinished = 0;
		QString requestAllQuantitiesFromPrescription = "SELECT etatcommande FROM commande WHERE idordonnance = " + QString::number(idPrescription);
		QSqlQuery responseAllQuantitiesFromPrescription = db->selectDB(requestAllQuantitiesFromPrescription);

		int responseAllQuantitiesFromPrescriptionRowNumber;
		if (responseAllQuantitiesFromPrescription.last()) {
			responseAllQuantitiesFromPrescriptionRowNumber = responseAllQuantitiesFromPrescription.at() + 1; responseAllQuantitiesFromPrescription.at() + 1;
			responseAllQuantitiesFromPrescription.first();
			responseAllQuantitiesFromPrescription.previous();
		}

		if (responseAllQuantitiesFromPrescriptionRowNumber > 0) {
			for (int i = 0; i < responseAllQuantitiesFromPrescriptionRowNumber; i++) {
				responseAllQuantitiesFromPrescription.next();
				bool state = responseAllQuantitiesFromPrescription.value(0).toBool();
				if (state)
					rowFinished++;
			}
		}
		if (rowFinished == responseAllQuantitiesFromPrescriptionRowNumber) {
			endOfOrder = true;
		}
		
	}
}

bool Scanner::orderAvailable()
{
	QString requestOrderAvailable = "SELECT * FROM commande WHERE etatcommande = false";
	QSqlQuery responseOrderAvailable = db->selectDB(requestOrderAvailable);
	int responseOrderAvailableRowNumber = 0;
	if (responseOrderAvailable.last())
	{
		responseOrderAvailableRowNumber = responseOrderAvailable.at() + 1;
		responseOrderAvailable.first();
		responseOrderAvailable.previous();
	}
	if(responseOrderAvailableRowNumber > 0)
		return true;
	return false;
}

bool Scanner::getEndOfOrder()
{
	return endOfOrder;
}

void Scanner::setEndOfOrder(bool state)
{
	endOfOrder = state;
}

void Scanner::setMedicineScanned(bool state)
{
	medicineScanned = state;
}

bool Scanner::getMedicineScanned()
{
	return medicineScanned;
}

void Scanner::receiveMessage() 
{
	if (scanner->canReadLine())
	{
		QByteArray data = scanner->readLine();
		barCode = QString(data).remove("\r\n");
		scanner->clear();
		hasScanned = true;
		medicineScanned = true;
	}
}

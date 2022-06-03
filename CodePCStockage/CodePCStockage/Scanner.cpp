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

QVector<QVariant> Scanner::identifyMed(QString barCode)
{
	QVector<QVariant> medInfo;
	QString requestSelectBarcode = "SELECT * FROM Commande, Medicament WHERE Commande.IDMedicament = Medicament.idmedicament AND Medicament.code_barre = " + barCode;
	QSqlQuery responseVerifyingMed = db->selectDB(requestSelectBarcode);
	if (responseVerifyingMed.size() > 0) {
		QString requestMedInformation = "SELECT CASE WHEN medicament.longueur > medicament.hauteur AND medicament.longueur > medicament.largeur THEN medicament.longueur  WHEN medicament.hauteur > medicament.largeur THEN medicament.hauteur ELSE medicament.largeur END AS 'taille', ordonnance.idcaisse, medicament.individuel, medicament.poids, commande.idcommande FROM medicament, ordonnance, commande WHERE medicament.idmedicament = commande.idmedicament AND commande.idordonnance = ordonnance.idordonnance ORDER by ordonnance.date DESC LIMIT 1";
		QSqlQuery responseMedInformation = db->selectDB(requestMedInformation);

		if (responseMedInformation.next()) {
			medInfo.push_back(responseMedInformation.value(0).toFloat());
			medInfo.push_back(responseMedInformation.value(1).toInt());
			medInfo.push_back(responseMedInformation.value(2).toInt());
			medInfo.push_back(responseMedInformation.value(3).toFloat());
			medInfo.push_back(responseMedInformation.value(4).toInt());
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

void Scanner::getNumberOfOrder()
{

}

void Scanner::receiveMessage() 
{
	if (scanner->canReadLine())
	{
		QByteArray data = scanner->readLine();
		barCode = QString(data).remove("\r\n");
		scanner->clear();
		hasScanned = true;
	}
}

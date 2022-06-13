#include "pharmacien.h"
#include <QtWidgets/QApplication>
#include "bilanCommande.h"
#include "bilanFinancier.h"
#include "ajoutmedicament.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pharmacien w;
	bilanCommande z(&w);
	bilanFinancier y(&w);
	ajoutmedicament s(&w);


	
	w.setBilanCommande(&z);
	w.setBilanFinancier(&y);
	w.setajoutmedicament(&s);
    w.show();
    return a.exec();
}

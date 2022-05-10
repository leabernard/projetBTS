#include "pharmacien.h"
#include <QtWidgets/QApplication>
#include "bilanCommande.h"
#include "bilanFinancier.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pharmacien w;
	bilanCommande z(&w);
	//bilanFinancier y(&w);
	w.setBilanCommande(&z);
	//w.setBilanFinancier(&y);
    w.show();
    return a.exec();
}

#include "pharmacien.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pharmacien w;
    w.show();
    return a.exec();
}

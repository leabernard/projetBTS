/********************************************************************************
** Form generated from reading UI file 'pharmacien.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHARMACIEN_H
#define UI_PHARMACIEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pharmacienClass
{
public:
    QWidget *centralWidget;
    QCommandLinkButton *validerCommande;
    QSpinBox *quantite;
    QLabel *labelQuantite;
    QComboBox *listeDeMedoc;
    QLabel *image;
    QLineEdit *lineEditNom;
    QLabel *labelStatue;
    QPushButton *boutonPatient;
    QLabel *label;
    QLineEdit *lineEditIDCaisse;
    QLabel *label_2;
    QLabel *label_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *pharmacienClass)
    {
        if (pharmacienClass->objectName().isEmpty())
            pharmacienClass->setObjectName(QString::fromUtf8("pharmacienClass"));
        pharmacienClass->resize(600, 400);
        centralWidget = new QWidget(pharmacienClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        validerCommande = new QCommandLinkButton(centralWidget);
        validerCommande->setObjectName(QString::fromUtf8("validerCommande"));
        validerCommande->setGeometry(QRect(10, 270, 185, 41));
        quantite = new QSpinBox(centralWidget);
        quantite->setObjectName(QString::fromUtf8("quantite"));
        quantite->setGeometry(QRect(220, 130, 42, 22));
        labelQuantite = new QLabel(centralWidget);
        labelQuantite->setObjectName(QString::fromUtf8("labelQuantite"));
        labelQuantite->setGeometry(QRect(280, 130, 71, 31));
        listeDeMedoc = new QComboBox(centralWidget);
        listeDeMedoc->setObjectName(QString::fromUtf8("listeDeMedoc"));
        listeDeMedoc->setGeometry(QRect(40, 130, 121, 21));
        image = new QLabel(centralWidget);
        image->setObjectName(QString::fromUtf8("image"));
        image->setGeometry(QRect(440, 20, 141, 121));
        image->setPixmap(QPixmap(QString::fromUtf8(":/pharmacien/pharmacie.png")));
        image->setScaledContents(true);
        lineEditNom = new QLineEdit(centralWidget);
        lineEditNom->setObjectName(QString::fromUtf8("lineEditNom"));
        lineEditNom->setGeometry(QRect(20, 20, 113, 20));
        labelStatue = new QLabel(centralWidget);
        labelStatue->setObjectName(QString::fromUtf8("labelStatue"));
        labelStatue->setGeometry(QRect(260, 250, 331, 51));
        boutonPatient = new QPushButton(centralWidget);
        boutonPatient->setObjectName(QString::fromUtf8("boutonPatient"));
        boutonPatient->setGeometry(QRect(290, 20, 131, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(340, 140, 71, 21));
        lineEditIDCaisse = new QLineEdit(centralWidget);
        lineEditIDCaisse->setObjectName(QString::fromUtf8("lineEditIDCaisse"));
        lineEditIDCaisse->setGeometry(QRect(160, 20, 113, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 40, 71, 16));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(170, 40, 71, 16));
        pharmacienClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(pharmacienClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        pharmacienClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(pharmacienClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        pharmacienClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(pharmacienClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        pharmacienClass->setStatusBar(statusBar);

        retranslateUi(pharmacienClass);
        QObject::connect(boutonPatient, SIGNAL(clicked()), pharmacienClass, SLOT(patient()));
        QObject::connect(validerCommande, SIGNAL(clicked()), pharmacienClass, SLOT(commande()));

        QMetaObject::connectSlotsByName(pharmacienClass);
    } // setupUi

    void retranslateUi(QMainWindow *pharmacienClass)
    {
        pharmacienClass->setWindowTitle(QCoreApplication::translate("pharmacienClass", "pharmacien", nullptr));
        validerCommande->setText(QCoreApplication::translate("pharmacienClass", "valider commande", nullptr));
        labelQuantite->setText(QCoreApplication::translate("pharmacienClass", "  Quantit\303\251", nullptr));
        image->setText(QString());
        labelStatue->setText(QString());
        boutonPatient->setText(QCoreApplication::translate("pharmacienClass", "Nom du patient", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("pharmacienClass", "Nom Patient", nullptr));
        label_3->setText(QCoreApplication::translate("pharmacienClass", "ID Caisse", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pharmacienClass: public Ui_pharmacienClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHARMACIEN_H

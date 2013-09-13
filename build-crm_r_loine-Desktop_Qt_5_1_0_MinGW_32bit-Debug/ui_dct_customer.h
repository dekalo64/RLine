/********************************************************************************
** Form generated from reading UI file 'dct_customer.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCT_CUSTOMER_H
#define UI_DCT_CUSTOMER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCustomer
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutUnionViews;
    QVBoxLayout *vLayoutUnionPartnerHuman;
    QHBoxLayout *hLayoutSearchToItem;
    QHBoxLayout *hLayoutInformation;
    QLabel *labelCurrentUser;
    QLabel *labelViewState;

    void setupUi(QWidget *CCustomer)
    {
        if (CCustomer->objectName().isEmpty())
            CCustomer->setObjectName(QString::fromUtf8("CCustomer"));
        CCustomer->resize(751, 212);
        gridLayout = new QGridLayout(CCustomer);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hLayoutUnionViews = new QHBoxLayout();
        hLayoutUnionViews->setObjectName(QString::fromUtf8("hLayoutUnionViews"));
        vLayoutUnionPartnerHuman = new QVBoxLayout();
        vLayoutUnionPartnerHuman->setSpacing(0);
        vLayoutUnionPartnerHuman->setObjectName(QString::fromUtf8("vLayoutUnionPartnerHuman"));

        hLayoutUnionViews->addLayout(vLayoutUnionPartnerHuman);


        gridLayout->addLayout(hLayoutUnionViews, 2, 0, 1, 2);

        hLayoutSearchToItem = new QHBoxLayout();
        hLayoutSearchToItem->setObjectName(QString::fromUtf8("hLayoutSearchToItem"));

        gridLayout->addLayout(hLayoutSearchToItem, 0, 0, 1, 2);

        hLayoutInformation = new QHBoxLayout();
        hLayoutInformation->setObjectName(QString::fromUtf8("hLayoutInformation"));
        labelCurrentUser = new QLabel(CCustomer);
        labelCurrentUser->setObjectName(QString::fromUtf8("labelCurrentUser"));

        hLayoutInformation->addWidget(labelCurrentUser, 0, Qt::AlignLeft|Qt::AlignBottom);

        labelViewState = new QLabel(CCustomer);
        labelViewState->setObjectName(QString::fromUtf8("labelViewState"));

        hLayoutInformation->addWidget(labelViewState, 0, Qt::AlignRight|Qt::AlignBottom);


        gridLayout->addLayout(hLayoutInformation, 3, 0, 1, 2);


        retranslateUi(CCustomer);

        QMetaObject::connectSlotsByName(CCustomer);
    } // setupUi

    void retranslateUi(QWidget *CCustomer)
    {
        CCustomer->setWindowTitle(QApplication::translate("CCustomer", "Form", 0, QApplication::UnicodeUTF8));
        labelCurrentUser->setText(QApplication::translate("CCustomer", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelViewState->setText(QApplication::translate("CCustomer", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCustomer: public Ui_CCustomer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCT_CUSTOMER_H

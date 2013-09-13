/********************************************************************************
** Form generated from reading UI file 'dct_countrycity.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCT_COUNTRYCITY_H
#define UI_DCT_COUNTRYCITY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCountryCity
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutSearchToItem;
    QHBoxLayout *hLayoutInformation;
    QLabel *labelCurrentUser;
    QLabel *labelViewState;
    QHBoxLayout *hLayoutTreeView;

    void setupUi(QWidget *CCountryCity)
    {
        if (CCountryCity->objectName().isEmpty())
            CCountryCity->setObjectName(QString::fromUtf8("CCountryCity"));
        CCountryCity->resize(780, 258);
        gridLayout = new QGridLayout(CCountryCity);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hLayoutSearchToItem = new QHBoxLayout();
#ifndef Q_OS_MAC
        hLayoutSearchToItem->setContentsMargins(0, 0, 0, 0);
#endif
        hLayoutSearchToItem->setObjectName(QString::fromUtf8("hLayoutSearchToItem"));
        hLayoutSearchToItem->setSizeConstraint(QLayout::SetDefaultConstraint);

        gridLayout->addLayout(hLayoutSearchToItem, 0, 0, 1, 1);

        hLayoutInformation = new QHBoxLayout();
        hLayoutInformation->setObjectName(QString::fromUtf8("hLayoutInformation"));
        labelCurrentUser = new QLabel(CCountryCity);
        labelCurrentUser->setObjectName(QString::fromUtf8("labelCurrentUser"));
        labelCurrentUser->setFrameShape(QFrame::NoFrame);

        hLayoutInformation->addWidget(labelCurrentUser, 0, Qt::AlignLeft|Qt::AlignBottom);

        labelViewState = new QLabel(CCountryCity);
        labelViewState->setObjectName(QString::fromUtf8("labelViewState"));
        labelViewState->setMaximumSize(QSize(16777215, 15));
        labelViewState->setFrameShape(QFrame::NoFrame);

        hLayoutInformation->addWidget(labelViewState, 0, Qt::AlignRight|Qt::AlignBottom);


        gridLayout->addLayout(hLayoutInformation, 2, 0, 1, 1);

        hLayoutTreeView = new QHBoxLayout();
        hLayoutTreeView->setObjectName(QString::fromUtf8("hLayoutTreeView"));

        gridLayout->addLayout(hLayoutTreeView, 1, 0, 1, 1);


        retranslateUi(CCountryCity);

        QMetaObject::connectSlotsByName(CCountryCity);
    } // setupUi

    void retranslateUi(QWidget *CCountryCity)
    {
        CCountryCity->setWindowTitle(QApplication::translate("CCountryCity", "Form", 0, QApplication::UnicodeUTF8));
        labelCurrentUser->setText(QApplication::translate("CCountryCity", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelViewState->setText(QApplication::translate("CCountryCity", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCountryCity: public Ui_CCountryCity {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCT_COUNTRYCITY_H

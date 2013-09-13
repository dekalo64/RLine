/********************************************************************************
** Form generated from reading UI file 'dct_cppsst.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCT_CPPSST_H
#define UI_DCT_CPPSST_H

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

class Ui_CCppsst
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutSearchToItem;
    QVBoxLayout *vLayoutTreeView;
    QHBoxLayout *hLayoutInformation;
    QLabel *labelCurrentUser;
    QLabel *labelViewState;

    void setupUi(QWidget *CCppsst)
    {
        if (CCppsst->objectName().isEmpty())
            CCppsst->setObjectName(QString::fromUtf8("CCppsst"));
        CCppsst->resize(737, 334);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CCppsst->sizePolicy().hasHeightForWidth());
        CCppsst->setSizePolicy(sizePolicy);
        CCppsst->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(CCppsst);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hLayoutSearchToItem = new QHBoxLayout();
        hLayoutSearchToItem->setObjectName(QString::fromUtf8("hLayoutSearchToItem"));

        gridLayout->addLayout(hLayoutSearchToItem, 0, 0, 1, 2);

        vLayoutTreeView = new QVBoxLayout();
        vLayoutTreeView->setObjectName(QString::fromUtf8("vLayoutTreeView"));

        gridLayout->addLayout(vLayoutTreeView, 1, 0, 1, 2);

        hLayoutInformation = new QHBoxLayout();
        hLayoutInformation->setObjectName(QString::fromUtf8("hLayoutInformation"));
        labelCurrentUser = new QLabel(CCppsst);
        labelCurrentUser->setObjectName(QString::fromUtf8("labelCurrentUser"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        labelCurrentUser->setFont(font);

        hLayoutInformation->addWidget(labelCurrentUser, 0, Qt::AlignLeft|Qt::AlignBottom);

        labelViewState = new QLabel(CCppsst);
        labelViewState->setObjectName(QString::fromUtf8("labelViewState"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(8);
        font1.setBold(false);
        font1.setWeight(50);
        labelViewState->setFont(font1);

        hLayoutInformation->addWidget(labelViewState, 0, Qt::AlignRight|Qt::AlignBottom);


        gridLayout->addLayout(hLayoutInformation, 2, 0, 1, 2);


        retranslateUi(CCppsst);

        QMetaObject::connectSlotsByName(CCppsst);
    } // setupUi

    void retranslateUi(QWidget *CCppsst)
    {
        CCppsst->setWindowTitle(QString());
        labelCurrentUser->setText(QApplication::translate("CCppsst", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelViewState->setText(QApplication::translate("CCppsst", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCppsst: public Ui_CCppsst {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCT_CPPSST_H

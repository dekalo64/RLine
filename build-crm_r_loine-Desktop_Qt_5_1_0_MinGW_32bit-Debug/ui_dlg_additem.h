/********************************************************************************
** Form generated from reading UI file 'dlg_additem.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_ADDITEM_H
#define UI_DLG_ADDITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_AddItem
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *m_newChoose;
    QGridLayout *gridLayout;
    QRadioButton *radioNewCatalog;
    QRadioButton *radioNewSubCatalog;
    QHBoxLayout *hLayoutOkCancel;
    QPushButton *buttonCancel;
    QPushButton *buttonSave;

    void setupUi(QDialog *AddItem)
    {
        if (AddItem->objectName().isEmpty())
            AddItem->setObjectName(QString::fromUtf8("AddItem"));
        AddItem->setWindowModality(Qt::WindowModal);
        AddItem->resize(250, 114);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AddItem->sizePolicy().hasHeightForWidth());
        AddItem->setSizePolicy(sizePolicy);
        AddItem->setMinimumSize(QSize(250, 114));
        AddItem->setMaximumSize(QSize(250, 114));
        AddItem->setModal(true);
        gridLayout_2 = new QGridLayout(AddItem);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        m_newChoose = new QGroupBox(AddItem);
        m_newChoose->setObjectName(QString::fromUtf8("m_newChoose"));
        m_newChoose->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        m_newChoose->setFlat(false);
        m_newChoose->setCheckable(false);
        gridLayout = new QGridLayout(m_newChoose);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        radioNewCatalog = new QRadioButton(m_newChoose);
        radioNewCatalog->setObjectName(QString::fromUtf8("radioNewCatalog"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        radioNewCatalog->setFont(font);
        radioNewCatalog->setIconSize(QSize(16, 16));

        gridLayout->addWidget(radioNewCatalog, 0, 0, 1, 1);

        radioNewSubCatalog = new QRadioButton(m_newChoose);
        radioNewSubCatalog->setObjectName(QString::fromUtf8("radioNewSubCatalog"));
        radioNewSubCatalog->setFont(font);
        radioNewSubCatalog->setIconSize(QSize(16, 16));

        gridLayout->addWidget(radioNewSubCatalog, 1, 0, 1, 1);


        gridLayout_2->addWidget(m_newChoose, 0, 0, 1, 2);

        hLayoutOkCancel = new QHBoxLayout();
        hLayoutOkCancel->setObjectName(QString::fromUtf8("hLayoutOkCancel"));
        buttonCancel = new QPushButton(AddItem);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setFont(font);

        hLayoutOkCancel->addWidget(buttonCancel);

        buttonSave = new QPushButton(AddItem);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setFont(font);

        hLayoutOkCancel->addWidget(buttonSave);


        gridLayout_2->addLayout(hLayoutOkCancel, 4, 0, 1, 2);


        retranslateUi(AddItem);

        QMetaObject::connectSlotsByName(AddItem);
    } // setupUi

    void retranslateUi(QDialog *AddItem)
    {
        AddItem->setWindowTitle(QApplication::translate("AddItem", "Dialog", 0, QApplication::UnicodeUTF8));
        m_newChoose->setTitle(QString());
        radioNewCatalog->setText(QApplication::translate("AddItem", "\320\235\320\276\320\262\321\213\320\271  \320\272\320\260\321\202\320\260\320\273\320\276\320\263", 0, QApplication::UnicodeUTF8));
        radioNewSubCatalog->setText(QApplication::translate("AddItem", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\276\320\264\320\272\320\260\321\202\320\260\320\273\320\276\320\263", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("AddItem", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("AddItem", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddItem: public Ui_AddItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_ADDITEM_H

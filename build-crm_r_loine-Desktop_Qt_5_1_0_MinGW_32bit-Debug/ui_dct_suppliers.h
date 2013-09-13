/********************************************************************************
** Form generated from reading UI file 'dct_suppliers.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCT_SUPPLIERS_H
#define UI_DCT_SUPPLIERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Suppliers
{
public:
    QGridLayout *gridLayout;
    QTreeView *treeViewSuppliers;

    void setupUi(QWidget *Suppliers)
    {
        if (Suppliers->objectName().isEmpty())
            Suppliers->setObjectName(QString::fromUtf8("Suppliers"));
        Suppliers->resize(726, 373);
        gridLayout = new QGridLayout(Suppliers);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeViewSuppliers = new QTreeView(Suppliers);
        treeViewSuppliers->setObjectName(QString::fromUtf8("treeViewSuppliers"));

        gridLayout->addWidget(treeViewSuppliers, 0, 0, 1, 1);


        retranslateUi(Suppliers);

        QMetaObject::connectSlotsByName(Suppliers);
    } // setupUi

    void retranslateUi(QWidget *Suppliers)
    {
        Suppliers->setWindowTitle(QApplication::translate("Suppliers", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Suppliers: public Ui_Suppliers {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCT_SUPPLIERS_H

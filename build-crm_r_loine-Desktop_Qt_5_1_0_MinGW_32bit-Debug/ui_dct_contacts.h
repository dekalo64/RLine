/********************************************************************************
** Form generated from reading UI file 'dct_contacts.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCT_CONTACTS_H
#define UI_DCT_CONTACTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Contacts
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonNew;
    QPushButton *buttonEdit;
    QPushButton *buttonDelete;
    QPushButton *buttonRefresh;
    QPushButton *buttonPrint;
    QPushButton *buttonSearch;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelContractor;
    QComboBox *comboContractor;
    QLabel *labelUsers;
    QComboBox *comboUsers;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkSuppliers;
    QCheckBox *checkClients;
    QCheckBox *checkProducers;
    QVBoxLayout *verticalLayout;
    QTreeView *treeViewContacts;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelActions;
    QLabel *labelComments;
    QHBoxLayout *horizontalLayout_2;
    QTreeView *treeViewActions;
    QTextEdit *textEditComments;

    void setupUi(QWidget *Contacts)
    {
        if (Contacts->objectName().isEmpty())
            Contacts->setObjectName(QString::fromUtf8("Contacts"));
        Contacts->resize(786, 464);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Contacts->sizePolicy().hasHeightForWidth());
        Contacts->setSizePolicy(sizePolicy);
        Contacts->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(Contacts);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonNew = new QPushButton(Contacts);
        buttonNew->setObjectName(QString::fromUtf8("buttonNew"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        buttonNew->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/forms/data/img/png/new_document.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonNew->setIcon(icon);

        horizontalLayout->addWidget(buttonNew);

        buttonEdit = new QPushButton(Contacts);
        buttonEdit->setObjectName(QString::fromUtf8("buttonEdit"));
        buttonEdit->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/forms/data/img/png/document_edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonEdit->setIcon(icon1);

        horizontalLayout->addWidget(buttonEdit);

        buttonDelete = new QPushButton(Contacts);
        buttonDelete->setObjectName(QString::fromUtf8("buttonDelete"));
        buttonDelete->setFont(font);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/forms/data/img/png/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDelete->setIcon(icon2);

        horizontalLayout->addWidget(buttonDelete);

        buttonRefresh = new QPushButton(Contacts);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        buttonRefresh->setFont(font);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/forms/data/img/png/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon3);

        horizontalLayout->addWidget(buttonRefresh);

        buttonPrint = new QPushButton(Contacts);
        buttonPrint->setObjectName(QString::fromUtf8("buttonPrint"));
        buttonPrint->setFont(font);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/forms/data/img/png/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonPrint->setIcon(icon4);

        horizontalLayout->addWidget(buttonPrint);

        buttonSearch = new QPushButton(Contacts);
        buttonSearch->setObjectName(QString::fromUtf8("buttonSearch"));
        buttonSearch->setFont(font);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/forms/data/img/png/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearch->setIcon(icon5);

        horizontalLayout->addWidget(buttonSearch);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        labelContractor = new QLabel(Contacts);
        labelContractor->setObjectName(QString::fromUtf8("labelContractor"));
        labelContractor->setFont(font);

        horizontalLayout_4->addWidget(labelContractor);

        comboContractor = new QComboBox(Contacts);
        comboContractor->setObjectName(QString::fromUtf8("comboContractor"));
        comboContractor->setEditable(true);

        horizontalLayout_4->addWidget(comboContractor);

        labelUsers = new QLabel(Contacts);
        labelUsers->setObjectName(QString::fromUtf8("labelUsers"));
        labelUsers->setFont(font);

        horizontalLayout_4->addWidget(labelUsers);

        comboUsers = new QComboBox(Contacts);
        comboUsers->setObjectName(QString::fromUtf8("comboUsers"));
        comboUsers->setEditable(true);

        horizontalLayout_4->addWidget(comboUsers);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        checkSuppliers = new QCheckBox(Contacts);
        checkSuppliers->setObjectName(QString::fromUtf8("checkSuppliers"));
        checkSuppliers->setFont(font);

        horizontalLayout_4->addWidget(checkSuppliers);

        checkClients = new QCheckBox(Contacts);
        checkClients->setObjectName(QString::fromUtf8("checkClients"));
        checkClients->setFont(font);

        horizontalLayout_4->addWidget(checkClients);

        checkProducers = new QCheckBox(Contacts);
        checkProducers->setObjectName(QString::fromUtf8("checkProducers"));
        checkProducers->setFont(font);

        horizontalLayout_4->addWidget(checkProducers);


        gridLayout->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeViewContacts = new QTreeView(Contacts);
        treeViewContacts->setObjectName(QString::fromUtf8("treeViewContacts"));

        verticalLayout->addWidget(treeViewContacts);


        gridLayout->addLayout(verticalLayout, 2, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelActions = new QLabel(Contacts);
        labelActions->setObjectName(QString::fromUtf8("labelActions"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        labelActions->setFont(font1);

        horizontalLayout_3->addWidget(labelActions);

        labelComments = new QLabel(Contacts);
        labelComments->setObjectName(QString::fromUtf8("labelComments"));
        labelComments->setFont(font);

        horizontalLayout_3->addWidget(labelComments);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        treeViewActions = new QTreeView(Contacts);
        treeViewActions->setObjectName(QString::fromUtf8("treeViewActions"));

        horizontalLayout_2->addWidget(treeViewActions);

        textEditComments = new QTextEdit(Contacts);
        textEditComments->setObjectName(QString::fromUtf8("textEditComments"));

        horizontalLayout_2->addWidget(textEditComments);


        gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        labelContractor->setBuddy(comboContractor);
        labelUsers->setBuddy(comboUsers);
#endif // QT_NO_SHORTCUT

        retranslateUi(Contacts);

        QMetaObject::connectSlotsByName(Contacts);
    } // setupUi

    void retranslateUi(QWidget *Contacts)
    {
        Contacts->setWindowTitle(QString());
        buttonNew->setText(QApplication::translate("Contacts", "\320\235\320\276\320\262\321\213\320\271", 0, QApplication::UnicodeUTF8));
        buttonEdit->setText(QApplication::translate("Contacts", "\320\237\321\200\320\260\320\262\320\272\320\260", 0, QApplication::UnicodeUTF8));
        buttonDelete->setText(QApplication::translate("Contacts", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QApplication::translate("Contacts", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonPrint->setText(QApplication::translate("Contacts", "\320\237\320\265\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonSearch->setText(QApplication::translate("Contacts", "\320\237\320\276\320\270\321\201\320\272", 0, QApplication::UnicodeUTF8));
        labelContractor->setText(QApplication::translate("Contacts", "\320\232\320\276\320\275\321\202\321\200\320\260\320\263\320\265\320\275\321\202", 0, QApplication::UnicodeUTF8));
        labelUsers->setText(QApplication::translate("Contacts", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\270", 0, QApplication::UnicodeUTF8));
        checkSuppliers->setText(QApplication::translate("Contacts", "\320\237\320\276\321\201\321\202\320\260\320\262\321\211\320\270\320\272\320\270", 0, QApplication::UnicodeUTF8));
        checkClients->setText(QApplication::translate("Contacts", "\320\232\320\273\320\270\320\265\320\275\321\202\321\213", 0, QApplication::UnicodeUTF8));
        checkProducers->setText(QApplication::translate("Contacts", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\264\320\270\321\202\320\265\320\273\320\270", 0, QApplication::UnicodeUTF8));
        labelActions->setText(QApplication::translate("Contacts", "\320\241\320\276\320\261\321\213\321\202\320\270\321\217/\320\227\320\260\320\264\320\260\321\207\320\270", 0, QApplication::UnicodeUTF8));
        labelComments->setText(QApplication::translate("Contacts", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\270", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Contacts: public Ui_Contacts {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCT_CONTACTS_H

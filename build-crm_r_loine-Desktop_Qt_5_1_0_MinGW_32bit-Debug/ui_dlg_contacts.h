/********************************************************************************
** Form generated from reading UI file 'dlg_contacts.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_CONTACTS_H
#define UI_DLG_CONTACTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactsDialog
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabGeneral;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBoxMain;
    QGridLayout *gridLayout_8;
    QLabel *labelContractor;
    QLineEdit *lineEditContractor;
    QLabel *labelSubdivision;
    QComboBox *comboSubdivision;
    QLabel *labelContractorType;
    QComboBox *comboContractorType;
    QToolButton *buttonContractor;
    QGroupBox *groupBoxContactName;
    QGridLayout *gridLayout_2;
    QDateEdit *dateBirth;
    QLabel *labelResponsibleUser;
    QLabel *labelPosition;
    QComboBox *comboPriorityType;
    QLineEdit *lineEditName;
    QLabel *labelDateBirth;
    QLabel *labelSurName;
    QLabel *labelName;
    QLabel *labelContactType;
    QLabel *labelSex;
    QComboBox *comboSex;
    QLineEdit *lineEditSurName;
    QComboBox *comboPosition;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *comboContactType;
    QLabel *labelPriorityType;
    QLabel *labelResponsible;
    QWidget *tabConnection;
    QGridLayout *gridLayout_9;
    QLabel *labelCity;
    QLineEdit *lineEditCity;
    QTreeView *treeViewConnection;
    QLineEdit *lineEditICQ;
    QLineEdit *lineEditAddress;
    QLabel *labelAddress;
    QLineEdit *lineEditSIP;
    QLabel *labelSIP;
    QLineEdit *lineEditSkype;
    QLabel *labelSkype;
    QLabel *labelICQ;
    QCheckBox *checkGeneralContact;
    QWidget *m_tabAdditionally;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBoxComments;
    QGridLayout *gridLayout_4;
    QTextEdit *textEditComments;
    QLabel *labelDateRegistration;
    QDateEdit *dateRegistration;
    QWidget *m_tabDocuments;
    QGridLayout *gridLayout_5;
    QComboBox *comboDocumentPeriodSelect;
    QDateEdit *dateDocumentPeriod;
    QTreeView *treeViewDocument;
    QWidget *m_tabRelations;
    QGridLayout *gridLayout_6;
    QComboBox *comboRelationsPeriodSelect;
    QTreeView *treeViewRelations;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonNew;
    QPushButton *buttonSave;
    QPushButton *buttonCancel;

    void setupUi(QDialog *ContactsDialog)
    {
        if (ContactsDialog->objectName().isEmpty())
            ContactsDialog->setObjectName(QString::fromUtf8("ContactsDialog"));
        ContactsDialog->resize(561, 377);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ContactsDialog->sizePolicy().hasHeightForWidth());
        ContactsDialog->setSizePolicy(sizePolicy);
        ContactsDialog->setMinimumSize(QSize(561, 0));
        ContactsDialog->setMaximumSize(QSize(561, 16777215));
        ContactsDialog->setModal(false);
        gridLayout = new QGridLayout(ContactsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(ContactsDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        tabWidget->setFont(font);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabWidget->setDocumentMode(false);
        tabGeneral = new QWidget();
        tabGeneral->setObjectName(QString::fromUtf8("tabGeneral"));
        gridLayout_7 = new QGridLayout(tabGeneral);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        groupBoxMain = new QGroupBox(tabGeneral);
        groupBoxMain->setObjectName(QString::fromUtf8("groupBoxMain"));
        groupBoxMain->setFlat(false);
        gridLayout_8 = new QGridLayout(groupBoxMain);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        labelContractor = new QLabel(groupBoxMain);
        labelContractor->setObjectName(QString::fromUtf8("labelContractor"));
        labelContractor->setFont(font);

        gridLayout_8->addWidget(labelContractor, 0, 0, 1, 1);

        lineEditContractor = new QLineEdit(groupBoxMain);
        lineEditContractor->setObjectName(QString::fromUtf8("lineEditContractor"));
        lineEditContractor->setReadOnly(true);

        gridLayout_8->addWidget(lineEditContractor, 0, 2, 1, 1);

        labelSubdivision = new QLabel(groupBoxMain);
        labelSubdivision->setObjectName(QString::fromUtf8("labelSubdivision"));
        labelSubdivision->setFont(font);

        gridLayout_8->addWidget(labelSubdivision, 1, 0, 1, 1);

        comboSubdivision = new QComboBox(groupBoxMain);
        comboSubdivision->setObjectName(QString::fromUtf8("comboSubdivision"));
        comboSubdivision->setEditable(true);

        gridLayout_8->addWidget(comboSubdivision, 1, 1, 1, 2);

        labelContractorType = new QLabel(groupBoxMain);
        labelContractorType->setObjectName(QString::fromUtf8("labelContractorType"));
        labelContractorType->setFont(font);

        gridLayout_8->addWidget(labelContractorType, 2, 0, 1, 1);

        comboContractorType = new QComboBox(groupBoxMain);
        comboContractorType->setObjectName(QString::fromUtf8("comboContractorType"));
        comboContractorType->setEditable(true);

        gridLayout_8->addWidget(comboContractorType, 2, 1, 1, 2);

        buttonContractor = new QToolButton(groupBoxMain);
        buttonContractor->setObjectName(QString::fromUtf8("buttonContractor"));

        gridLayout_8->addWidget(buttonContractor, 0, 1, 1, 1);


        gridLayout_7->addWidget(groupBoxMain, 0, 0, 1, 1);

        groupBoxContactName = new QGroupBox(tabGeneral);
        groupBoxContactName->setObjectName(QString::fromUtf8("groupBoxContactName"));
        gridLayout_2 = new QGridLayout(groupBoxContactName);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        dateBirth = new QDateEdit(groupBoxContactName);
        dateBirth->setObjectName(QString::fromUtf8("dateBirth"));
        dateBirth->setDateTime(QDateTime(QDate(2013, 1, 1), QTime(0, 0, 0)));
        dateBirth->setCalendarPopup(true);

        gridLayout_2->addWidget(dateBirth, 3, 1, 1, 1);

        labelResponsibleUser = new QLabel(groupBoxContactName);
        labelResponsibleUser->setObjectName(QString::fromUtf8("labelResponsibleUser"));

        gridLayout_2->addWidget(labelResponsibleUser, 4, 1, 1, 1);

        labelPosition = new QLabel(groupBoxContactName);
        labelPosition->setObjectName(QString::fromUtf8("labelPosition"));

        gridLayout_2->addWidget(labelPosition, 2, 0, 1, 1);

        comboPriorityType = new QComboBox(groupBoxContactName);
        comboPriorityType->setObjectName(QString::fromUtf8("comboPriorityType"));
        comboPriorityType->setEditable(true);

        gridLayout_2->addWidget(comboPriorityType, 1, 4, 1, 1);

        lineEditName = new QLineEdit(groupBoxContactName);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gridLayout_2->addWidget(lineEditName, 1, 1, 1, 1);

        labelDateBirth = new QLabel(groupBoxContactName);
        labelDateBirth->setObjectName(QString::fromUtf8("labelDateBirth"));

        gridLayout_2->addWidget(labelDateBirth, 3, 0, 1, 1);

        labelSurName = new QLabel(groupBoxContactName);
        labelSurName->setObjectName(QString::fromUtf8("labelSurName"));

        gridLayout_2->addWidget(labelSurName, 0, 0, 1, 1);

        labelName = new QLabel(groupBoxContactName);
        labelName->setObjectName(QString::fromUtf8("labelName"));

        gridLayout_2->addWidget(labelName, 1, 0, 1, 1);

        labelContactType = new QLabel(groupBoxContactName);
        labelContactType->setObjectName(QString::fromUtf8("labelContactType"));

        gridLayout_2->addWidget(labelContactType, 0, 3, 1, 1);

        labelSex = new QLabel(groupBoxContactName);
        labelSex->setObjectName(QString::fromUtf8("labelSex"));

        gridLayout_2->addWidget(labelSex, 2, 3, 1, 1);

        comboSex = new QComboBox(groupBoxContactName);
        comboSex->setObjectName(QString::fromUtf8("comboSex"));
        comboSex->setEditable(true);

        gridLayout_2->addWidget(comboSex, 2, 4, 1, 1);

        lineEditSurName = new QLineEdit(groupBoxContactName);
        lineEditSurName->setObjectName(QString::fromUtf8("lineEditSurName"));

        gridLayout_2->addWidget(lineEditSurName, 0, 1, 1, 1);

        comboPosition = new QComboBox(groupBoxContactName);
        comboPosition->setObjectName(QString::fromUtf8("comboPosition"));
        comboPosition->setEditable(true);

        gridLayout_2->addWidget(comboPosition, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        comboContactType = new QComboBox(groupBoxContactName);
        comboContactType->setObjectName(QString::fromUtf8("comboContactType"));
        comboContactType->setEditable(true);

        gridLayout_2->addWidget(comboContactType, 0, 4, 1, 1);

        labelPriorityType = new QLabel(groupBoxContactName);
        labelPriorityType->setObjectName(QString::fromUtf8("labelPriorityType"));

        gridLayout_2->addWidget(labelPriorityType, 1, 3, 1, 1);

        labelResponsible = new QLabel(groupBoxContactName);
        labelResponsible->setObjectName(QString::fromUtf8("labelResponsible"));

        gridLayout_2->addWidget(labelResponsible, 4, 0, 1, 1);


        gridLayout_7->addWidget(groupBoxContactName, 1, 0, 1, 1);

        tabWidget->addTab(tabGeneral, QString());
        tabConnection = new QWidget();
        tabConnection->setObjectName(QString::fromUtf8("tabConnection"));
        gridLayout_9 = new QGridLayout(tabConnection);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        labelCity = new QLabel(tabConnection);
        labelCity->setObjectName(QString::fromUtf8("labelCity"));

        gridLayout_9->addWidget(labelCity, 3, 0, 1, 1, Qt::AlignLeft);

        lineEditCity = new QLineEdit(tabConnection);
        lineEditCity->setObjectName(QString::fromUtf8("lineEditCity"));

        gridLayout_9->addWidget(lineEditCity, 3, 1, 1, 1);

        treeViewConnection = new QTreeView(tabConnection);
        treeViewConnection->setObjectName(QString::fromUtf8("treeViewConnection"));

        gridLayout_9->addWidget(treeViewConnection, 0, 0, 1, 8);

        lineEditICQ = new QLineEdit(tabConnection);
        lineEditICQ->setObjectName(QString::fromUtf8("lineEditICQ"));

        gridLayout_9->addWidget(lineEditICQ, 3, 4, 1, 1);

        lineEditAddress = new QLineEdit(tabConnection);
        lineEditAddress->setObjectName(QString::fromUtf8("lineEditAddress"));

        gridLayout_9->addWidget(lineEditAddress, 5, 1, 1, 1);

        labelAddress = new QLabel(tabConnection);
        labelAddress->setObjectName(QString::fromUtf8("labelAddress"));

        gridLayout_9->addWidget(labelAddress, 5, 0, 1, 1, Qt::AlignLeft);

        lineEditSIP = new QLineEdit(tabConnection);
        lineEditSIP->setObjectName(QString::fromUtf8("lineEditSIP"));

        gridLayout_9->addWidget(lineEditSIP, 3, 6, 1, 1);

        labelSIP = new QLabel(tabConnection);
        labelSIP->setObjectName(QString::fromUtf8("labelSIP"));

        gridLayout_9->addWidget(labelSIP, 3, 5, 1, 1);

        lineEditSkype = new QLineEdit(tabConnection);
        lineEditSkype->setObjectName(QString::fromUtf8("lineEditSkype"));

        gridLayout_9->addWidget(lineEditSkype, 5, 4, 1, 1);

        labelSkype = new QLabel(tabConnection);
        labelSkype->setObjectName(QString::fromUtf8("labelSkype"));

        gridLayout_9->addWidget(labelSkype, 5, 3, 1, 1);

        labelICQ = new QLabel(tabConnection);
        labelICQ->setObjectName(QString::fromUtf8("labelICQ"));

        gridLayout_9->addWidget(labelICQ, 3, 3, 1, 1, Qt::AlignHCenter);

        checkGeneralContact = new QCheckBox(tabConnection);
        checkGeneralContact->setObjectName(QString::fromUtf8("checkGeneralContact"));

        gridLayout_9->addWidget(checkGeneralContact, 5, 6, 1, 1);

        tabWidget->addTab(tabConnection, QString());
        m_tabAdditionally = new QWidget();
        m_tabAdditionally->setObjectName(QString::fromUtf8("m_tabAdditionally"));
        gridLayout_3 = new QGridLayout(m_tabAdditionally);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBoxComments = new QGroupBox(m_tabAdditionally);
        groupBoxComments->setObjectName(QString::fromUtf8("groupBoxComments"));
        gridLayout_4 = new QGridLayout(groupBoxComments);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textEditComments = new QTextEdit(groupBoxComments);
        textEditComments->setObjectName(QString::fromUtf8("textEditComments"));

        gridLayout_4->addWidget(textEditComments, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBoxComments, 0, 0, 1, 2);

        labelDateRegistration = new QLabel(m_tabAdditionally);
        labelDateRegistration->setObjectName(QString::fromUtf8("labelDateRegistration"));

        gridLayout_3->addWidget(labelDateRegistration, 1, 0, 1, 1);

        dateRegistration = new QDateEdit(m_tabAdditionally);
        dateRegistration->setObjectName(QString::fromUtf8("dateRegistration"));
        dateRegistration->setCalendarPopup(true);

        gridLayout_3->addWidget(dateRegistration, 1, 1, 1, 1);

        tabWidget->addTab(m_tabAdditionally, QString());
        m_tabDocuments = new QWidget();
        m_tabDocuments->setObjectName(QString::fromUtf8("m_tabDocuments"));
        gridLayout_5 = new QGridLayout(m_tabDocuments);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        comboDocumentPeriodSelect = new QComboBox(m_tabDocuments);
        comboDocumentPeriodSelect->setObjectName(QString::fromUtf8("comboDocumentPeriodSelect"));
        comboDocumentPeriodSelect->setEditable(true);

        gridLayout_5->addWidget(comboDocumentPeriodSelect, 0, 0, 1, 1);

        dateDocumentPeriod = new QDateEdit(m_tabDocuments);
        dateDocumentPeriod->setObjectName(QString::fromUtf8("dateDocumentPeriod"));
        dateDocumentPeriod->setCalendarPopup(true);

        gridLayout_5->addWidget(dateDocumentPeriod, 0, 1, 1, 1);

        treeViewDocument = new QTreeView(m_tabDocuments);
        treeViewDocument->setObjectName(QString::fromUtf8("treeViewDocument"));

        gridLayout_5->addWidget(treeViewDocument, 1, 0, 1, 2);

        tabWidget->addTab(m_tabDocuments, QString());
        m_tabRelations = new QWidget();
        m_tabRelations->setObjectName(QString::fromUtf8("m_tabRelations"));
        gridLayout_6 = new QGridLayout(m_tabRelations);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        comboRelationsPeriodSelect = new QComboBox(m_tabRelations);
        comboRelationsPeriodSelect->setObjectName(QString::fromUtf8("comboRelationsPeriodSelect"));
        comboRelationsPeriodSelect->setEditable(true);

        gridLayout_6->addWidget(comboRelationsPeriodSelect, 0, 0, 1, 1);

        treeViewRelations = new QTreeView(m_tabRelations);
        treeViewRelations->setObjectName(QString::fromUtf8("treeViewRelations"));

        gridLayout_6->addWidget(treeViewRelations, 1, 0, 1, 1);

        tabWidget->addTab(m_tabRelations, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonNew = new QPushButton(ContactsDialog);
        buttonNew->setObjectName(QString::fromUtf8("buttonNew"));
        buttonNew->setFont(font);

        horizontalLayout->addWidget(buttonNew);

        buttonSave = new QPushButton(ContactsDialog);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setFont(font);

        horizontalLayout->addWidget(buttonSave);

        buttonCancel = new QPushButton(ContactsDialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setFont(font);

        horizontalLayout->addWidget(buttonCancel);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(ContactsDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ContactsDialog);
    } // setupUi

    void retranslateUi(QDialog *ContactsDialog)
    {
        ContactsDialog->setWindowTitle(QApplication::translate("ContactsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBoxMain->setTitle(QApplication::translate("ContactsDialog", "\320\236\321\201\320\275\320\276\320\262\320\275\320\276\320\265", 0, QApplication::UnicodeUTF8));
        labelContractor->setText(QApplication::translate("ContactsDialog", "\320\232\320\276\320\275\321\202\321\200\320\260\320\263\320\265\320\275\321\202", 0, QApplication::UnicodeUTF8));
        labelSubdivision->setText(QApplication::translate("ContactsDialog", "\320\237\320\276\320\264\321\200\320\260\320\267\320\264\320\265\320\273\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        labelContractorType->setText(QApplication::translate("ContactsDialog", "\320\242\320\270\320\277 \320\272\320\276\320\275\321\202\321\200\320\260\320\263\320\265\320\275\321\202\320\260", 0, QApplication::UnicodeUTF8));
        buttonContractor->setText(QApplication::translate("ContactsDialog", "...", 0, QApplication::UnicodeUTF8));
        groupBoxContactName->setTitle(QApplication::translate("ContactsDialog", "\320\232\320\276\320\275\321\202\320\260\320\272\321\202\320\275\320\276\320\265 \320\273\320\270\321\206\320\276", 0, QApplication::UnicodeUTF8));
        labelResponsibleUser->setText(QApplication::translate("ContactsDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelPosition->setText(QApplication::translate("ContactsDialog", "\320\224\320\276\320\273\320\266\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));
        labelDateBirth->setText(QApplication::translate("ContactsDialog", "\320\224\320\260\321\202\320\260 \321\200\320\276\320\266\320\264\320\265\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        labelSurName->setText(QApplication::translate("ContactsDialog", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217", 0, QApplication::UnicodeUTF8));
        labelName->setText(QApplication::translate("ContactsDialog", "\320\230\320\274\321\217", 0, QApplication::UnicodeUTF8));
        labelContactType->setText(QApplication::translate("ContactsDialog", "\320\242\320\270\320\277", 0, QApplication::UnicodeUTF8));
        labelSex->setText(QApplication::translate("ContactsDialog", "\320\237\320\276\320\273", 0, QApplication::UnicodeUTF8));
        labelPriorityType->setText(QApplication::translate("ContactsDialog", "\320\237\321\200\320\270\320\276\321\200\320\270\321\202\320\265\321\202", 0, QApplication::UnicodeUTF8));
        labelResponsible->setText(QApplication::translate("ContactsDialog", "\320\236\321\202\320\262\320\265\321\202\321\201\321\202\320\262\320\265\320\275\320\275\321\213\320\271", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabGeneral), QApplication::translate("ContactsDialog", "\320\236\320\261\321\211\320\270\320\265", 0, QApplication::UnicodeUTF8));
        labelCity->setText(QApplication::translate("ContactsDialog", "\320\223\320\276\321\200\320\276\320\264", 0, QApplication::UnicodeUTF8));
        labelAddress->setText(QApplication::translate("ContactsDialog", "\320\220\320\264\321\200\320\265\321\201", 0, QApplication::UnicodeUTF8));
        labelSIP->setText(QApplication::translate("ContactsDialog", "SIP", 0, QApplication::UnicodeUTF8));
        labelSkype->setText(QApplication::translate("ContactsDialog", "Skype", 0, QApplication::UnicodeUTF8));
        labelICQ->setText(QApplication::translate("ContactsDialog", "ICQ", 0, QApplication::UnicodeUTF8));
        checkGeneralContact->setText(QApplication::translate("ContactsDialog", "\320\236\321\201\320\275\320\276\320\262\320\275\320\276\320\271 \320\272\320\276\320\275\321\202\320\260\320\272\321\202", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabConnection), QApplication::translate("ContactsDialog", "\320\241\320\262\321\217\320\267\321\214", 0, QApplication::UnicodeUTF8));
        groupBoxComments->setTitle(QApplication::translate("ContactsDialog", "\320\232\320\276\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\270", 0, QApplication::UnicodeUTF8));
        labelDateRegistration->setText(QApplication::translate("ContactsDialog", "\320\224\320\260\321\202\320\260 \321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_tabAdditionally), QApplication::translate("ContactsDialog", "\320\224\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\320\276", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_tabDocuments), QApplication::translate("ContactsDialog", "\320\224\320\276\320\272\321\203\320\274\320\265\320\275\321\202\321\213", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_tabRelations), QApplication::translate("ContactsDialog", "\320\222\320\267\320\260\320\270\320\274\320\276\320\276\321\202\320\275\320\276\321\210\320\265\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        buttonNew->setText(QApplication::translate("ContactsDialog", "\320\235\320\276\320\262\321\213\320\271", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("ContactsDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("ContactsDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ContactsDialog: public Ui_ContactsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_CONTACTS_H

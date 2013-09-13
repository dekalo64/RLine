/********************************************************************************
** Form generated from reading UI file 'dlg_cppsst.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_CPPSST_H
#define UI_DLG_CPPSST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CCppsstDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gLayoutDictionaryTemplate;
    QLabel *labelItem;
    QLineEdit *lineEditName;
    QLabel *labelIcon;
    QComboBox *comboBoxIcon;
    QHBoxLayout *hLayoutActual;
    QCheckBox *checkBoxActual;
    QHBoxLayout *hLayoutOkCancel;
    QPushButton *buttonSave;
    QPushButton *buttonCancel;
    QFrame *line;
    QGridLayout *gLayoutInformation;
    QLabel *labelUserI;
    QLabel *labelUserD;
    QLabel *labelDateI;
    QLabel *labelDateD;

    void setupUi(QDialog *CCppsstDialog)
    {
        if (CCppsstDialog->objectName().isEmpty())
            CCppsstDialog->setObjectName(QString::fromUtf8("CCppsstDialog"));
        CCppsstDialog->setWindowModality(Qt::WindowModal);
        CCppsstDialog->resize(310, 176);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CCppsstDialog->sizePolicy().hasHeightForWidth());
        CCppsstDialog->setSizePolicy(sizePolicy);
        CCppsstDialog->setMinimumSize(QSize(310, 176));
        CCppsstDialog->setMaximumSize(QSize(310, 176));
        CCppsstDialog->setAcceptDrops(true);
        CCppsstDialog->setModal(true);
        gridLayout_2 = new QGridLayout(CCppsstDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gLayoutDictionaryTemplate = new QGridLayout();
        gLayoutDictionaryTemplate->setObjectName(QString::fromUtf8("gLayoutDictionaryTemplate"));
        labelItem = new QLabel(CCppsstDialog);
        labelItem->setObjectName(QString::fromUtf8("labelItem"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelItem->sizePolicy().hasHeightForWidth());
        labelItem->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        labelItem->setFont(font);

        gLayoutDictionaryTemplate->addWidget(labelItem, 0, 0, 1, 1);

        lineEditName = new QLineEdit(CCppsstDialog);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gLayoutDictionaryTemplate->addWidget(lineEditName, 0, 1, 1, 1);

        labelIcon = new QLabel(CCppsstDialog);
        labelIcon->setObjectName(QString::fromUtf8("labelIcon"));
        labelIcon->setFont(font);

        gLayoutDictionaryTemplate->addWidget(labelIcon, 1, 0, 1, 1);

        comboBoxIcon = new QComboBox(CCppsstDialog);
        comboBoxIcon->setObjectName(QString::fromUtf8("comboBoxIcon"));
        comboBoxIcon->setEditable(true);

        gLayoutDictionaryTemplate->addWidget(comboBoxIcon, 1, 1, 1, 1);


        gridLayout_2->addLayout(gLayoutDictionaryTemplate, 0, 0, 1, 1);

        hLayoutActual = new QHBoxLayout();
        hLayoutActual->setObjectName(QString::fromUtf8("hLayoutActual"));
        checkBoxActual = new QCheckBox(CCppsstDialog);
        checkBoxActual->setObjectName(QString::fromUtf8("checkBoxActual"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(checkBoxActual->sizePolicy().hasHeightForWidth());
        checkBoxActual->setSizePolicy(sizePolicy2);
        checkBoxActual->setFont(font);

        hLayoutActual->addWidget(checkBoxActual, 0, Qt::AlignRight);


        gridLayout_2->addLayout(hLayoutActual, 1, 0, 1, 1);

        hLayoutOkCancel = new QHBoxLayout();
        hLayoutOkCancel->setObjectName(QString::fromUtf8("hLayoutOkCancel"));
        buttonSave = new QPushButton(CCppsstDialog);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setFont(font);
        buttonSave->setDefault(false);
        buttonSave->setFlat(false);

        hLayoutOkCancel->addWidget(buttonSave);

        buttonCancel = new QPushButton(CCppsstDialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setFont(font);

        hLayoutOkCancel->addWidget(buttonCancel);


        gridLayout_2->addLayout(hLayoutOkCancel, 2, 0, 1, 1);

        line = new QFrame(CCppsstDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShadow(QFrame::Raised);
        line->setFrameShape(QFrame::HLine);

        gridLayout_2->addWidget(line, 3, 0, 1, 1);

        gLayoutInformation = new QGridLayout();
        gLayoutInformation->setObjectName(QString::fromUtf8("gLayoutInformation"));
        labelUserI = new QLabel(CCppsstDialog);
        labelUserI->setObjectName(QString::fromUtf8("labelUserI"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        labelUserI->setFont(font1);
        labelUserI->setFrameShadow(QFrame::Plain);

        gLayoutInformation->addWidget(labelUserI, 0, 0, 1, 1, Qt::AlignLeft);

        labelUserD = new QLabel(CCppsstDialog);
        labelUserD->setObjectName(QString::fromUtf8("labelUserD"));
        labelUserD->setFont(font);

        gLayoutInformation->addWidget(labelUserD, 1, 0, 1, 1, Qt::AlignLeft);

        labelDateI = new QLabel(CCppsstDialog);
        labelDateI->setObjectName(QString::fromUtf8("labelDateI"));
        labelDateI->setFont(font1);

        gLayoutInformation->addWidget(labelDateI, 0, 1, 1, 1, Qt::AlignRight);

        labelDateD = new QLabel(CCppsstDialog);
        labelDateD->setObjectName(QString::fromUtf8("labelDateD"));
        labelDateD->setFont(font);

        gLayoutInformation->addWidget(labelDateD, 1, 1, 1, 1, Qt::AlignRight);


        gridLayout_2->addLayout(gLayoutInformation, 4, 0, 1, 1);


        retranslateUi(CCppsstDialog);

        QMetaObject::connectSlotsByName(CCppsstDialog);
    } // setupUi

    void retranslateUi(QDialog *CCppsstDialog)
    {
        CCppsstDialog->setWindowTitle(QApplication::translate("CCppsstDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        labelItem->setText(QApplication::translate("CCppsstDialog", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        labelIcon->setText(QApplication::translate("CCppsstDialog", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214", 0, QApplication::UnicodeUTF8));
        checkBoxActual->setText(QApplication::translate("CCppsstDialog", "\320\220\320\272\321\202\321\203\320\260\320\273\321\214\320\275\320\260", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("CCppsstDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CCppsstDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        labelUserI->setText(QApplication::translate("CCppsstDialog", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        labelUserD->setText(QApplication::translate("CCppsstDialog", "User", 0, QApplication::UnicodeUTF8));
        labelDateI->setText(QApplication::translate("CCppsstDialog", "\320\224\320\260\321\202\320\260", 0, QApplication::UnicodeUTF8));
        labelDateD->setText(QApplication::translate("CCppsstDialog", "Date", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCppsstDialog: public Ui_CCppsstDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_CPPSST_H

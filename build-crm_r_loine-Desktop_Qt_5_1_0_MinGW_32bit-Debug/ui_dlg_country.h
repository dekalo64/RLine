/********************************************************************************
** Form generated from reading UI file 'dlg_country.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_COUNTRY_H
#define UI_DLG_COUNTRY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CountryDialog
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gLayoutCountry;
    QLineEdit *lineEditName;
    QLabel *labelNameEng;
    QLabel *labelName;
    QLineEdit *lineEditNameEng;
    QLabel *labelCityCode;
    QLineEdit *lineEditCityCode;
    QHBoxLayout *hLayoutActual;
    QCheckBox *checkBoxActual;
    QHBoxLayout *hLayoutOkCancel;
    QPushButton *buttonSave;
    QPushButton *buttonCancel;
    QFrame *line;
    QGridLayout *gLayoutInformation;
    QLabel *labelUserD;
    QLabel *labelUserI;
    QLabel *labelDateI;
    QLabel *labelDateD;

    void setupUi(QDialog *CountryDialog)
    {
        if (CountryDialog->objectName().isEmpty())
            CountryDialog->setObjectName(QString::fromUtf8("CountryDialog"));
        CountryDialog->setWindowModality(Qt::WindowModal);
        CountryDialog->resize(310, 199);
        CountryDialog->setMinimumSize(QSize(310, 199));
        CountryDialog->setMaximumSize(QSize(310, 201));
        CountryDialog->setModal(true);
        gridLayout_3 = new QGridLayout(CountryDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gLayoutCountry = new QGridLayout();
        gLayoutCountry->setObjectName(QString::fromUtf8("gLayoutCountry"));
        lineEditName = new QLineEdit(CountryDialog);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gLayoutCountry->addWidget(lineEditName, 0, 1, 1, 1);

        labelNameEng = new QLabel(CountryDialog);
        labelNameEng->setObjectName(QString::fromUtf8("labelNameEng"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        labelNameEng->setFont(font);

        gLayoutCountry->addWidget(labelNameEng, 1, 0, 1, 1);

        labelName = new QLabel(CountryDialog);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        labelName->setFont(font);

        gLayoutCountry->addWidget(labelName, 0, 0, 1, 1);

        lineEditNameEng = new QLineEdit(CountryDialog);
        lineEditNameEng->setObjectName(QString::fromUtf8("lineEditNameEng"));

        gLayoutCountry->addWidget(lineEditNameEng, 1, 1, 1, 1);

        labelCityCode = new QLabel(CountryDialog);
        labelCityCode->setObjectName(QString::fromUtf8("labelCityCode"));
        labelCityCode->setFont(font);

        gLayoutCountry->addWidget(labelCityCode, 2, 0, 1, 1);

        lineEditCityCode = new QLineEdit(CountryDialog);
        lineEditCityCode->setObjectName(QString::fromUtf8("lineEditCityCode"));

        gLayoutCountry->addWidget(lineEditCityCode, 2, 1, 1, 1);


        gridLayout_3->addLayout(gLayoutCountry, 0, 0, 1, 1);

        hLayoutActual = new QHBoxLayout();
        hLayoutActual->setObjectName(QString::fromUtf8("hLayoutActual"));
        checkBoxActual = new QCheckBox(CountryDialog);
        checkBoxActual->setObjectName(QString::fromUtf8("checkBoxActual"));
        checkBoxActual->setFont(font);

        hLayoutActual->addWidget(checkBoxActual, 0, Qt::AlignRight);


        gridLayout_3->addLayout(hLayoutActual, 1, 0, 1, 1);

        hLayoutOkCancel = new QHBoxLayout();
        hLayoutOkCancel->setObjectName(QString::fromUtf8("hLayoutOkCancel"));
        buttonSave = new QPushButton(CountryDialog);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setFont(font);

        hLayoutOkCancel->addWidget(buttonSave);

        buttonCancel = new QPushButton(CountryDialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setFont(font);

        hLayoutOkCancel->addWidget(buttonCancel);


        gridLayout_3->addLayout(hLayoutOkCancel, 2, 0, 1, 1);

        line = new QFrame(CountryDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 3, 0, 1, 1);

        gLayoutInformation = new QGridLayout();
        gLayoutInformation->setObjectName(QString::fromUtf8("gLayoutInformation"));
        labelUserD = new QLabel(CountryDialog);
        labelUserD->setObjectName(QString::fromUtf8("labelUserD"));
        labelUserD->setFont(font);

        gLayoutInformation->addWidget(labelUserD, 1, 0, 1, 1, Qt::AlignLeft);

        labelUserI = new QLabel(CountryDialog);
        labelUserI->setObjectName(QString::fromUtf8("labelUserI"));

        gLayoutInformation->addWidget(labelUserI, 0, 0, 1, 1, Qt::AlignLeft);

        labelDateI = new QLabel(CountryDialog);
        labelDateI->setObjectName(QString::fromUtf8("labelDateI"));

        gLayoutInformation->addWidget(labelDateI, 0, 1, 1, 1, Qt::AlignRight);

        labelDateD = new QLabel(CountryDialog);
        labelDateD->setObjectName(QString::fromUtf8("labelDateD"));
        labelDateD->setFont(font);

        gLayoutInformation->addWidget(labelDateD, 1, 1, 1, 1, Qt::AlignRight);


        gridLayout_3->addLayout(gLayoutInformation, 4, 0, 1, 1);


        retranslateUi(CountryDialog);

        QMetaObject::connectSlotsByName(CountryDialog);
    } // setupUi

    void retranslateUi(QDialog *CountryDialog)
    {
        CountryDialog->setWindowTitle(QApplication::translate("CountryDialog", "\320\241\321\202\321\200\320\260\320\275\320\260, \321\200\320\265\320\263\320\270\320\276\320\275", 0, QApplication::UnicodeUTF8));
        labelNameEng->setText(QApplication::translate("CountryDialog", "Name", 0, QApplication::UnicodeUTF8));
        labelName->setText(QApplication::translate("CountryDialog", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        labelCityCode->setText(QApplication::translate("CountryDialog", "\320\232\320\276\320\264 \320\263\320\276\321\200\320\276\320\264\320\260 \342\204\226", 0, QApplication::UnicodeUTF8));
        checkBoxActual->setText(QApplication::translate("CountryDialog", "\320\220\320\272\321\202\321\203\320\260\320\273\321\214\320\275\320\260", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("CountryDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CountryDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        labelUserD->setText(QApplication::translate("CountryDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelUserI->setText(QApplication::translate("CountryDialog", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        labelDateI->setText(QApplication::translate("CountryDialog", "\320\224\320\260\321\202\320\260", 0, QApplication::UnicodeUTF8));
        labelDateD->setText(QApplication::translate("CountryDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CountryDialog: public Ui_CountryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_COUNTRY_H

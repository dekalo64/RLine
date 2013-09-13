/********************************************************************************
** Form generated from reading UI file 'dlg_city.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_CITY_H
#define UI_DLG_CITY_H

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

class Ui_CityDialog
{
public:
    QGridLayout *gridLayout_3;
    QHBoxLayout *hLayoutActual;
    QCheckBox *checkBoxActual;
    QHBoxLayout *hLayoutOkCancel;
    QPushButton *buttonSave;
    QPushButton *buttonCancel;
    QFrame *line;
    QGridLayout *gLayoutCountry;
    QLineEdit *lineEditCityCode;
    QLabel *labelName;
    QLabel *labelNameEng;
    QLabel *labelCityCode;
    QLabel *labelCountry;
    QLineEdit *lineEditCountryCode;
    QLineEdit *lineEditCountry;
    QLineEdit *lineEditName;
    QLineEdit *lineEditNameEng;
    QGridLayout *gLayoutInformation;
    QLabel *labelUserI;
    QLabel *labelUserD;
    QLabel *labelDateI;
    QLabel *labelDateD;

    void setupUi(QDialog *CityDialog)
    {
        if (CityDialog->objectName().isEmpty())
            CityDialog->setObjectName(QString::fromUtf8("CityDialog"));
        CityDialog->setWindowModality(Qt::WindowModal);
        CityDialog->resize(310, 227);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CityDialog->sizePolicy().hasHeightForWidth());
        CityDialog->setSizePolicy(sizePolicy);
        CityDialog->setMinimumSize(QSize(310, 227));
        CityDialog->setMaximumSize(QSize(310, 227));
        CityDialog->setModal(true);
        gridLayout_3 = new QGridLayout(CityDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        hLayoutActual = new QHBoxLayout();
        hLayoutActual->setObjectName(QString::fromUtf8("hLayoutActual"));
        checkBoxActual = new QCheckBox(CityDialog);
        checkBoxActual->setObjectName(QString::fromUtf8("checkBoxActual"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        checkBoxActual->setFont(font);

        hLayoutActual->addWidget(checkBoxActual, 0, Qt::AlignRight);


        gridLayout_3->addLayout(hLayoutActual, 1, 0, 1, 1);

        hLayoutOkCancel = new QHBoxLayout();
        hLayoutOkCancel->setObjectName(QString::fromUtf8("hLayoutOkCancel"));
        buttonSave = new QPushButton(CityDialog);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setFont(font);

        hLayoutOkCancel->addWidget(buttonSave);

        buttonCancel = new QPushButton(CityDialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setFont(font);

        hLayoutOkCancel->addWidget(buttonCancel);


        gridLayout_3->addLayout(hLayoutOkCancel, 2, 0, 1, 1);

        line = new QFrame(CityDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 3, 0, 1, 1);

        gLayoutCountry = new QGridLayout();
        gLayoutCountry->setObjectName(QString::fromUtf8("gLayoutCountry"));
        lineEditCityCode = new QLineEdit(CityDialog);
        lineEditCityCode->setObjectName(QString::fromUtf8("lineEditCityCode"));

        gLayoutCountry->addWidget(lineEditCityCode, 3, 2, 1, 1);

        labelName = new QLabel(CityDialog);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        labelName->setFont(font);

        gLayoutCountry->addWidget(labelName, 1, 0, 1, 1);

        labelNameEng = new QLabel(CityDialog);
        labelNameEng->setObjectName(QString::fromUtf8("labelNameEng"));
        labelNameEng->setFont(font);

        gLayoutCountry->addWidget(labelNameEng, 2, 0, 1, 1);

        labelCityCode = new QLabel(CityDialog);
        labelCityCode->setObjectName(QString::fromUtf8("labelCityCode"));
        labelCityCode->setFont(font);

        gLayoutCountry->addWidget(labelCityCode, 3, 0, 1, 1);

        labelCountry = new QLabel(CityDialog);
        labelCountry->setObjectName(QString::fromUtf8("labelCountry"));
        labelCountry->setFont(font);

        gLayoutCountry->addWidget(labelCountry, 0, 0, 1, 1);

        lineEditCountryCode = new QLineEdit(CityDialog);
        lineEditCountryCode->setObjectName(QString::fromUtf8("lineEditCountryCode"));
        lineEditCountryCode->setMinimumSize(QSize(50, 0));
        lineEditCountryCode->setMaximumSize(QSize(50, 16777215));

        gLayoutCountry->addWidget(lineEditCountryCode, 3, 1, 1, 1);

        lineEditCountry = new QLineEdit(CityDialog);
        lineEditCountry->setObjectName(QString::fromUtf8("lineEditCountry"));

        gLayoutCountry->addWidget(lineEditCountry, 0, 1, 1, 2);

        lineEditName = new QLineEdit(CityDialog);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));

        gLayoutCountry->addWidget(lineEditName, 1, 1, 1, 2);

        lineEditNameEng = new QLineEdit(CityDialog);
        lineEditNameEng->setObjectName(QString::fromUtf8("lineEditNameEng"));

        gLayoutCountry->addWidget(lineEditNameEng, 2, 1, 1, 2);


        gridLayout_3->addLayout(gLayoutCountry, 0, 0, 1, 1);

        gLayoutInformation = new QGridLayout();
        gLayoutInformation->setObjectName(QString::fromUtf8("gLayoutInformation"));
        labelUserI = new QLabel(CityDialog);
        labelUserI->setObjectName(QString::fromUtf8("labelUserI"));

        gLayoutInformation->addWidget(labelUserI, 0, 0, 1, 1, Qt::AlignLeft);

        labelUserD = new QLabel(CityDialog);
        labelUserD->setObjectName(QString::fromUtf8("labelUserD"));
        labelUserD->setFont(font);

        gLayoutInformation->addWidget(labelUserD, 1, 0, 1, 1, Qt::AlignLeft);

        labelDateI = new QLabel(CityDialog);
        labelDateI->setObjectName(QString::fromUtf8("labelDateI"));

        gLayoutInformation->addWidget(labelDateI, 0, 1, 1, 1, Qt::AlignRight);

        labelDateD = new QLabel(CityDialog);
        labelDateD->setObjectName(QString::fromUtf8("labelDateD"));
        labelDateD->setFont(font);

        gLayoutInformation->addWidget(labelDateD, 1, 1, 1, 1, Qt::AlignRight);


        gridLayout_3->addLayout(gLayoutInformation, 4, 0, 1, 1);


        retranslateUi(CityDialog);

        QMetaObject::connectSlotsByName(CityDialog);
    } // setupUi

    void retranslateUi(QDialog *CityDialog)
    {
        CityDialog->setWindowTitle(QApplication::translate("CityDialog", "\320\223\320\276\321\200\320\276\320\264", 0, QApplication::UnicodeUTF8));
        checkBoxActual->setText(QApplication::translate("CityDialog", "\320\220\320\272\321\202\321\203\320\260\320\273\321\214\320\275\320\260", 0, QApplication::UnicodeUTF8));
        buttonSave->setText(QApplication::translate("CityDialog", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("CityDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        labelName->setText(QApplication::translate("CityDialog", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        labelNameEng->setText(QApplication::translate("CityDialog", "Name", 0, QApplication::UnicodeUTF8));
        labelCityCode->setText(QApplication::translate("CityDialog", "\320\232\320\276\320\264 \320\263\320\276\321\200\320\276\320\264\320\260 \342\204\226", 0, QApplication::UnicodeUTF8));
        labelCountry->setText(QApplication::translate("CityDialog", "\320\241\321\202\321\200\320\260\320\275\320\260", 0, QApplication::UnicodeUTF8));
        labelUserI->setText(QApplication::translate("CityDialog", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        labelUserD->setText(QApplication::translate("CityDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelDateI->setText(QApplication::translate("CityDialog", "\320\224\320\260\321\202\320\260", 0, QApplication::UnicodeUTF8));
        labelDateD->setText(QApplication::translate("CityDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CityDialog: public Ui_CityDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_CITY_H

/********************************************************************************
** Form generated from reading UI file 'dlg_connection.ui'
**
** Created: Fri 13. Sep 00:52:01 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_CONNECTION_H
#define UI_DLG_CONNECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConnectionDialog
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEditUserName;
    QLabel *labelLogin;
    QLabel *labelPassword;
    QLineEdit *lineEditPassword;
    QHBoxLayout *hLayoutOkCancel;
    QPushButton *buttonCancel;
    QPushButton *buttonDatabaseConnect;
    QFrame *lineTop;
    QLabel *labelWindowDescription;

    void setupUi(QDialog *ConnectionDialog)
    {
        if (ConnectionDialog->objectName().isEmpty())
            ConnectionDialog->setObjectName(QString::fromUtf8("ConnectionDialog"));
        ConnectionDialog->setWindowModality(Qt::NonModal);
        ConnectionDialog->setEnabled(true);
        ConnectionDialog->resize(289, 136);
        ConnectionDialog->setMinimumSize(QSize(289, 136));
        ConnectionDialog->setMaximumSize(QSize(289, 136));
        QPalette palette;
        QBrush brush(QColor(51, 94, 168, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush);
        QBrush brush1(QColor(235, 233, 237, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush);
        ConnectionDialog->setPalette(palette);
        ConnectionDialog->setAutoFillBackground(false);
        ConnectionDialog->setSizeGripEnabled(false);
        ConnectionDialog->setModal(false);
        gridLayout = new QGridLayout(ConnectionDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lineEditUserName = new QLineEdit(ConnectionDialog);
        lineEditUserName->setObjectName(QString::fromUtf8("lineEditUserName"));
        lineEditUserName->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        lineEditUserName->setFont(font);

        gridLayout_2->addWidget(lineEditUserName, 0, 1, 1, 1);

        labelLogin = new QLabel(ConnectionDialog);
        labelLogin->setObjectName(QString::fromUtf8("labelLogin"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        labelLogin->setFont(font1);

        gridLayout_2->addWidget(labelLogin, 0, 0, 1, 1);

        labelPassword = new QLabel(ConnectionDialog);
        labelPassword->setObjectName(QString::fromUtf8("labelPassword"));
        labelPassword->setFont(font1);

        gridLayout_2->addWidget(labelPassword, 1, 0, 1, 1);

        lineEditPassword = new QLineEdit(ConnectionDialog);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditPassword->sizePolicy().hasHeightForWidth());
        lineEditPassword->setSizePolicy(sizePolicy);
        lineEditPassword->setMinimumSize(QSize(133, 0));
        lineEditPassword->setFont(font);
        lineEditPassword->setFrame(true);
        lineEditPassword->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(lineEditPassword, 1, 1, 1, 1);


        gridLayout->addLayout(gridLayout_2, 2, 0, 1, 1);

        hLayoutOkCancel = new QHBoxLayout();
        hLayoutOkCancel->setObjectName(QString::fromUtf8("hLayoutOkCancel"));
        buttonCancel = new QPushButton(ConnectionDialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setFont(font1);
        buttonCancel->setInputMethodHints(Qt::ImhNone);
        buttonCancel->setIconSize(QSize(16, 16));
        buttonCancel->setCheckable(false);
        buttonCancel->setAutoExclusive(false);
        buttonCancel->setFlat(false);

        hLayoutOkCancel->addWidget(buttonCancel);

        buttonDatabaseConnect = new QPushButton(ConnectionDialog);
        buttonDatabaseConnect->setObjectName(QString::fromUtf8("buttonDatabaseConnect"));
        buttonDatabaseConnect->setFont(font1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/connect/images/connect.ico"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDatabaseConnect->setIcon(icon);
        buttonDatabaseConnect->setDefault(true);
        buttonDatabaseConnect->setFlat(false);

        hLayoutOkCancel->addWidget(buttonDatabaseConnect);


        gridLayout->addLayout(hLayoutOkCancel, 4, 0, 1, 1);

        lineTop = new QFrame(ConnectionDialog);
        lineTop->setObjectName(QString::fromUtf8("lineTop"));
        lineTop->setFrameShape(QFrame::HLine);
        lineTop->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(lineTop, 1, 0, 1, 1);

        labelWindowDescription = new QLabel(ConnectionDialog);
        labelWindowDescription->setObjectName(QString::fromUtf8("labelWindowDescription"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(11);
        font2.setBold(true);
        font2.setWeight(75);
        labelWindowDescription->setFont(font2);

        gridLayout->addWidget(labelWindowDescription, 0, 0, 1, 1, Qt::AlignHCenter);


        retranslateUi(ConnectionDialog);

        QMetaObject::connectSlotsByName(ConnectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectionDialog)
    {
        ConnectionDialog->setWindowTitle(QString());
        labelLogin->setText(QApplication::translate("ConnectionDialog", "\320\233\320\276\320\263\320\270\320\275", 0, QApplication::UnicodeUTF8));
        labelPassword->setText(QApplication::translate("ConnectionDialog", "\320\237\320\260\321\200\320\276\320\273\321\214", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("ConnectionDialog", "\320\222\321\213\320\271\321\202\320\270", 0, QApplication::UnicodeUTF8));
        buttonDatabaseConnect->setText(QApplication::translate("ConnectionDialog", "\320\222\320\276\320\271\321\202\320\270", 0, QApplication::UnicodeUTF8));
        labelWindowDescription->setText(QApplication::translate("ConnectionDialog", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConnectionDialog: public Ui_ConnectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_CONNECTION_H

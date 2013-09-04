#pragma once

#ifndef LOGISTICAPPLICATION_H
#define LOGISTICAPPLICATION_H

#include "source/core/mdiwindow.h"
#include "source/dialog/connectiondialog.h"
#include "source/dictionary/positions.h"
#include "source/dictionary/tasktype.h"
#include "source/dictionary/contractortype.h"
#include "source/dictionary/status.h"
#include "source/dictionary/priorities.h"
#include "source/dictionary/countryandcity.h"
#include "source/dictionary/contacts.h"
#include "source/dictionary/suppliers.h"
#include "source/dictionary/customer.h"
#include "source/additionally/accountingoperation.h"

#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QCleanlooksStyle>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>

#define IS_VALID_PTR(ptr) ((ptr != (void*)0))

QT_BEGIN_NAMESPACE
class LogisticMainWindow;
class MdiWindow;
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(ConnectionDialog)

class LogisticApplication: public QApplication
{
    Q_OBJECT

public:
    explicit LogisticApplication(int &argc, char **argv);
    virtual ~LogisticApplication();

    static LogisticApplication  *instance();
    static MdiWindow            *createMdiMindow(const QString &title, const QIcon &icon);
    static ConnectionDialog     *connectionDialog(QWidget *parent);

    static Positions            *createPosition();
    static TaskType             *createTaskType();
    static ContractorType       *createContractorType();
    static Status               *createStatus();
    static Priorities           *createPriorities();
    static CountryAndCity       *createCountryAndCity();
    static Contacts             *createContacts();
    static Suppliers            *createSappliers();
    static CCustomer            *createCustomer();
//    static AccountingOperation  *createAccountingOperation();

    LogisticMainWindow          *mainWindow();

    inline QString driver() const;
    inline QString host()   const;
    QString dbname()        const;

    QSqlDatabase database;

    static Positions      *positions;
    static TaskType       *tasktype;
    static ContractorType *contractortype;
    static Status         *status;
    static Priorities     *priorities;
    static CountryAndCity *countryandcity;
    static Suppliers      *suppliers;
    static CCustomer      *customer;
//    static AccountingOperation *accountingoperation;

    static LogisticMainWindow *mainWnd;

Q_SIGNALS:
    void successConnection();

private slots:
    QSqlError slotAddConnection (const QString &user, const QString &passwd);
    void slotVisibleComponent   (bool visible);

private:
    static QSettings          *settings;
    static ConnectionDialog   *cntnDialog;

    static void setStyleWidget(QWidget *widget = 0, const QString &cssFile = QString());
};

#endif // LOGISTICAPPLICATION_H

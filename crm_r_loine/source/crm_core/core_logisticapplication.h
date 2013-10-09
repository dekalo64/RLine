#pragma once

#ifndef LOGISTICAPPLICATION_H
#define LOGISTICAPPLICATION_H

#include "source/crm_additionally/adl_communicate.h"

#include "source/crm_core/core_mdiwindow.h"
#include "source/crm_dialog/dlg_connection.h"
#include "source/crm_dictionary/dct_positions.h"
#include "source/crm_dictionary/dct_task.h"
#include "source/crm_dictionary/dct_contractor.h"
#include "source/crm_dictionary/dct_status.h"
#include "source/crm_dictionary/dct_priorities.h"
#include "source/crm_dictionary/dct_countrycity.h"
#include "source/crm_dictionary/dct_contacts.h"
#include "source/crm_dictionary/dct_suppliers.h"
#include "source/crm_dictionary/dct_customer.h"

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
class CLogisticMainWindow;
class MdiWindow;
QT_END_NAMESPACE

QT_FORWARD_DECLARE_CLASS(ConnectionDialog)

class CLogisticApplication: public QApplication
{
    Q_OBJECT

public:
    explicit CLogisticApplication(int &argc, char **argv);
    virtual ~CLogisticApplication();

// singleton
    static CLogisticApplication *instance();

    MdiWindow            *createMdiMindow(const QString &title, const QIcon &icon);
    ConnectionDialog     *connectionDialog(QWidget *parent = 0);

    CPositions           *createPosition();
    CTaskType            *createTaskType();
    CContractorType      *createContractorType();
    CStatus              *createStatus();
    CPriorities          *createPriorities();
    CCountryCity         *createCountryCity();
    CContacts            *createContacts();
    CSuppliers           *createSappliers();
    CCustomer            *createCustomer();

    inline QString driver() const{
            return settings->value("database/driver").toString();
    }
    inline QString host()   const{
            return settings->value("database/host").toString();
    }
    inline QString dbname() const{
            return settings->value("database/dbname").toString();
    }

    QSqlDatabase database;

    static CPositions      *position;
    static CTaskType       *tasktype;
    static CContractorType *contractortype;
    static CStatus         *status;
    static CPriorities     *priority;
    static CCountryCity    *countrycity;
    static CSuppliers      *supplier;
    static CCustomer       *customer;

Q_SIGNALS:
    void successConnection();

private slots:
    QSqlError slotAddConnection (const QString &user, const QString &passwd);
    void slotVisibleComponent   (bool visible);

private:
    static QSettings          *settings;
    static ConnectionDialog   *cntnDialog;

    void setStyleWidget(QWidget *widget = 0, const QString &cssFile = QString());
};

#endif // LOGISTICAPPLICATION_H

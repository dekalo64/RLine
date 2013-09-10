#ifndef CONTRACTORTYPE_H
#define CONTRACTORTYPE_H

#include "source/crm_dictionary/dct_cppsst.h"
#include "source/crm_dialog/dlg_cppsst.h"

#include <QtCore/QModelIndex>
#include <QtCore/QDateTime>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>

#define  CONTRACTOR_MODEL_COLUMN_COUNT 3

class ContractorType: public CCppsst
{
    Q_OBJECT

public:
    explicit ContractorType(QWidget *parent = 0);
    virtual ~ContractorType();

    bool actualRecords;

private:
    QStandardItemModel    *modelContractor;
    QItemSelectionModel   *modelSelectionContractor;

    void fillContractorModel(QSqlQuery &stored);
    bool fillFormSelectedRecord (void);

private slots:  
    void slotFillContractor       (void);

    void slotCreateEditDialog(const int &r);

    void slotActualRecords (const bool &actual);
    void slotFindContractor(const QString &text);
    void slotInsertOrUpdateRecords (void);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // CONTRACTORTYPE_H

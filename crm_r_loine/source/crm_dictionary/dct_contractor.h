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

class CContractorType: public CCppsst
{
    Q_OBJECT

public:
    explicit CContractorType(QWidget *parent = 0);
    virtual ~CContractorType();

    bool actualRecords;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelContractor;
    QItemSelectionModel   *modelSelectionContractor;

    void fillContractorModel(QSqlQuery &stored);
    bool fillListSelectedRecord (QList<QString> &param);

private slots:  
    void slotFillContractor       (void);

    void slotCreateEditDialog(const QString &action);
    void slotCreateEditDialog(void);

    void slotActualRecords (const bool &actual);
    void slotFindContractor(const QString &text);
    void slotInsertOrUpdateRecords (const QList<QString> &param);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // CONTRACTORTYPE_H

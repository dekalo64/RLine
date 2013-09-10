#ifndef TASKTYPE_H
#define TASKTYPE_H

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

#define  TASK_MODEL_COLUMN_COUNT 3

class TaskType : public CCppsst
{
    Q_OBJECT

public:
    explicit TaskType(QWidget *parent = 0);
    virtual ~TaskType();

    bool actualRecords;

private:
    QStandardItemModel    *modelTask;
    QItemSelectionModel   *modelSelectionTask;

    void fillTaskModel(QSqlQuery &stored);
    bool fillFormSelectedRecord (void);

private slots:
    void slotFillTask        (void);

    void slotCreateEditDialog(const int &r);

    void slotActualRecords (const bool &actual);
    void slotFindTask(const QString &text);
    void slotInsertOrUpdateRecords (void);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // TASKTYPE_H

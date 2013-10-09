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

class CTaskType : public CCppsst
{
    Q_OBJECT

public:
    explicit CTaskType(QWidget *parent = 0);
    virtual ~CTaskType();

    bool actualRecords;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelTask;
    QItemSelectionModel   *modelSelectionTask;

    void fillTaskModel(QSqlQuery &stored);
    bool fillListSelectedRecord (QList<QString> &param);

private slots:
    void slotFillTask        (void);

    void slotCreateEditDialog(const QString &action);
    void slotCreateEditDialog(void);

    void slotActualRecords (const bool &actual);
    void slotFindTask(const QString &text);
    void slotInsertOrUpdateRecords (const QList<QString> &param);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // TASKTYPE_H

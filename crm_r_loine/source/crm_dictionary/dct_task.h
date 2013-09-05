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

#define  MODEL_COLUMN_COUNT 5

class TaskType : public DictionaryTemplate
{
    Q_OBJECT

public:
    explicit TaskType(QWidget *parent = 0);
    virtual ~TaskType();

    bool actualRecords;

private:
    QStandardItemModel    *m_model;
    QSortFilterProxyModel *m_proxymodel;
    QItemSelectionModel   *m_selectionModel;

    void fillingModel(QSqlQuery &stored);

private slots:
    void slotInsertOrUpdateRecords (void);
    void slotSelectRecords         (bool actual);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);

    void slotPushSelectRecordData(void);

    void slotDataChanged         (const QModelIndex &index);
    void slotGetSelectionModel   (const QModelIndex &index);
};

#endif // TASKTYPE_H

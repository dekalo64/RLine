#ifndef PRIORITIES_H
#define PRIORITIES_H

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

#define  PRIORITIES_MODEL_COLUMN_COUNT 4

class Priorities: public CCppsst
{
    Q_OBJECT

public:
    explicit Priorities(QWidget *parent = 0);
    virtual ~Priorities();

    bool actualRecords;

private:
    QStandardItemModel    *modelPriorities;
    QItemSelectionModel   *modelSelectionPriorities;

    void fillPrioritiesModel(QSqlQuery &stored);
    bool fillFormSelectedRecord (void);

private slots:
    void slotFillPriorities        (void);

    void slotCreateEditDialog(const int &r);

    void slotActualRecords (const bool &actual);
    void slotFindPriorities(const QString &text);
    void slotInsertOrUpdateRecords (void);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // PRIORITIES_H

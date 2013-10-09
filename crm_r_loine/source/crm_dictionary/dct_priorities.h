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

class CPriorities: public CCppsst
{
    Q_OBJECT

public:
    explicit CPriorities(QWidget *parent = 0);
    virtual ~CPriorities();

    bool actualRecords;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelPriorities;
    QItemSelectionModel   *modelSelectionPriorities;

    void fillPrioritiesModel(QSqlQuery &stored);
    bool fillListSelectedRecord (QList<QString> &param);

private slots:
    void slotFillPriorities        (void);

    void slotCreateEditDialog(const QString &action);
    void slotCreateEditDialog(void);

    void slotActualRecords (const bool &actual);
    void slotFindPriorities(const QString &text);
    void slotInsertOrUpdateRecords (const QList<QString> &param);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // PRIORITIES_H

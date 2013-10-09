#ifndef STATUS_H
#define STATUS_H

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

#define  STATUS_MODEL_COLUMN_COUNT 3

class CStatus: public CCppsst
{
    Q_OBJECT

public:
    explicit CStatus(QWidget *parent = 0);
    virtual ~CStatus();

    bool actualRecords;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelStatus;
    QItemSelectionModel   *modelSelectionStatus;

    void fillStatusModel(QSqlQuery &stored);
    bool fillListSelectedRecord (QList<QString> &param);

private slots:
    void slotFillStatus       (void);

    void slotCreateEditDialog(const QString &action);
    void slotCreateEditDialog(void);

    void slotActualRecords (const bool &actual);
    void slotFindStatus(const QString &text);
    void slotInsertOrUpdateRecords (const QList<QString> &param);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // STATUS_H

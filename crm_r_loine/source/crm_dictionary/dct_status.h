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
<<<<<<< HEAD
#endif

<<<<<<< HEAD
=======
>>>>>>> parent of 4ea7787... Добавлено #if QT_VERSION для проверки с 5 версией

=======
>>>>>>> 7d34168ed62c3fc6f2c3e44c48dc517eb050ca29
#define  STATUS_MODEL_COLUMN_COUNT 3

class Status: public CCppsst
{
    Q_OBJECT

public:
    explicit Status(QWidget *parent = 0);
    virtual ~Status();

    bool actualRecords;

private:
    QStandardItemModel    *modelStatus;
    QItemSelectionModel   *modelSelectionStatus;

    void fillStatusModel(QSqlQuery &stored);
    bool fillFormSelectedRecord (void);

private slots:
    void slotFillStatus       (void);

    void slotCreateEditDialog(const int &r);

    void slotActualRecords (const bool &actual);
    void slotFindStatus(const QString &text);
    void slotInsertOrUpdateRecords (void);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // STATUS_H

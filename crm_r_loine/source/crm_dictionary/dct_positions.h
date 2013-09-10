#ifndef POSITIONS_H
#define POSITIONS_H

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

#define  POSITIONS_MODEL_COLUMN_COUNT 3

class Positions : public CCppsst
{
    Q_OBJECT

public:
    explicit Positions(QWidget *parent = 0);
    virtual ~Positions();

    bool actualRecords;

private:
    QStandardItemModel    *modelPositions;
    QItemSelectionModel   *modelSelectionPositions;

    void fillPositionsModel(QSqlQuery &stored);
    bool fillFormSelectedRecord (void);

private slots:
    void slotFillPositions         (void);

    void slotCreateEditDialog(const int &r);

    void slotActualRecords (const bool &actual);
    void slotFindPositions(const QString &text);
    void slotInsertOrUpdateRecords (void);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);
};

#endif // POSITIONS_H

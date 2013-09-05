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

class Positions : public DictionaryTemplate
{
    Q_OBJECT

public:
    explicit Positions(QWidget *parent = 0);
    virtual ~Positions();

    bool actualRecords;

private:
    QStandardItemModel    *m_model;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel   *m_selectionModel;

    void fillingModel(QSqlQuery &stored);

public slots:
    void slotInsertOrUpdateRecords (void);
    void slotSelectRecords         (bool actual);
    void slotCopyRecords           (void);
    void slotDeleteRecords         (void);
    void slotRefreshRecords        (void);

    void slotPushSelectRecordData(void);

    void slotDataChanged         (const QModelIndex &index);
    void slotGetSelectionModel   (const QModelIndex &index);
};

#endif // POSITIONS_H

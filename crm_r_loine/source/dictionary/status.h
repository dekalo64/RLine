#ifndef STATUS_H
#define STATUS_H

#include "source/dictionary/dictionarytemplate.h"
#include "source/dialog/dictionarydialog.h"

#include <QtCore/QModelIndex>
#include <QtCore/QDateTime>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>

class Status: public DictionaryTemplate
{
    Q_OBJECT

public:
    explicit Status(QWidget *parent = 0);
    virtual ~Status();

    bool actualRecords;
    bool actualRecordsRefresh;

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

#endif // STATUS_H

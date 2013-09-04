#ifndef CONTRACTORTYPE_H
#define CONTRACTORTYPE_H

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

class ContractorType: public DictionaryTemplate
{
    Q_OBJECT

public:
    explicit ContractorType(QWidget *parent = 0);
    virtual ~ContractorType();

    bool actualRecords;

private:
    QStandardItemModel    *m_model;
    QSortFilterProxyModel *m_proxyModel;
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

#endif // CONTRACTORTYPE_H

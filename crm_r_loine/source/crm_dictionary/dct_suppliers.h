#pragma once

#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"

#include <QtCore/QModelIndex>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeView>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>

namespace Ui {
class Suppliers;
}

class Suppliers: public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)

public:
    explicit Suppliers(QWidget *parent = 0);
    virtual ~Suppliers();

private:
    QStandardItemModel    *m_suppliersModel;
    QItemSelectionModel   *m_selectionSuppliersModel;

    void fillingSuppliersModel  (QSqlQuery &stored);
    inline QStandardItem *getRootItem(){
        return rootItem;
    }
//    QMultiMap<int, QString> setTreeData(QSqlQuery &stored, QStandardItem *p);
//    QMultiMap<QString, QString> namePerIndex(QSqlQuery &stored, QMultiMap<int, QString> &m);
//    void setTreeValue(QSqlQuery &stored, QStandardItem *p, int row);

//Q_SIGNALS:
//    void filterWildcard       (QString);
//    void pushSelectRecordData (void);

//private slots:
//    void slotSelectRecordsSuppliers (bool toggled);
//    void slotCopyRecords            (void);
//    void slotDeleteRecords          (void);
//    void slotDataChanged            (const QModelIndex &index);
//    void slotCreateEditDialog       (void);
//    void slotPushSelectRecordData   (void);
//    void slotInsertOrUpdateRecords  (void);

private:
    Ui::Suppliers              *ui;
    QStandardItem              *rootItem;
    RecordActionDatabase::Enum m_rad;
};

#endif // SUPPLIERS_H

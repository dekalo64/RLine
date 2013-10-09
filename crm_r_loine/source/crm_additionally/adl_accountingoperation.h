#pragma once

#ifndef CACCOUNTING_H
#define CACCOUNTING_H

#include "ui_dlg_refgrp.h"
#include "ui_dlg_inventories.h"

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"
#include "source/crm_dialog/dlg_additem.h"
#include "source/crm_dialog/dlg_inventories.h"
#include "source/crm_dialog/dlg_refgrp.h"

#include <QtCore/QModelIndex>
#include <QtCore/QDateTime>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QStandardItemModel>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeView>
#include <QtGui/QSplitter>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtGui/QListWidgetItem>

#define  REFGROUP_MODEL_COLUMN_COUNT     4
#define  INVENTORIES_MODEL_COLUMN_COUNT  3
#define  GROUP_TYPE_ACCOUNTING_OPERATION 114
#define  SKIP 0

namespace Ui {
class CAccountingOperation;
}

class CRefGroupTreeView;
class CInventoriesTreeView;

typedef struct {
    int codeInventories;
    int codeRefGroup;
    QString nameInventories;
    QString nameRefGroup;
} MoveInventories;

class CAccountingOperation : public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)

public:
    explicit CAccountingOperation(QWidget *parent = 0);
    virtual ~CAccountingOperation();

    bool actualRecords;

    QMenu *getContextMenu(void) const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelReferenceGroup;
    QStandardItemModel    *modelInventories;
    QItemSelectionModel   *modelSelectionReferenceGroup;
    QItemSelectionModel   *modelSelectionInventories;

    void fillReferenceGroupModel (QSqlQuery &stored, const QModelIndex &index);
    void fillInventoriesModel    (QSqlQuery &stored);
    bool fillFormSelectedRecord (void);

private slots:
    void slotFillGroup  (const QModelIndex &index);
    void slotFillInventories (const QModelIndex &index);
    void slotClearGroup (const QModelIndex &index);

    void slotActualRecords          (const bool &actual);
    void slotFindInventories        (const QString &text);
    void slotCutRecords             (void);
    void slotCopyRecords            (void);
    void slotPasteRecords           (void);
    void slotDeleteRecords          (void);
    void slotRefreshRecords         (void);
    void slotRefreshRecordsRefGroup (void);
    void slotRefreshRecordsInventory(void);
    void slotCreateEditDialog       (const int &r);
    void slotCreateEditDialog       (void);
    void slotInsertOrUpdateRecords  (void);

private:
    Ui::CAccountingOperation    *ui;

    QStandardItem               *root;
    CRefGroupTreeView           *treeRefGroup;
    CInventoriesTreeView        *treeInventories;
    CFilter                     *filter;

    RefGrpDialog                *referenceGroupDialog;
    InventoriesDialog           *inventoriesDialog;

    QWidget                     *focusedWidget;
    CAddItem                    *addItem;

    RecordActionDatabase::Enum   rad;
    MoveInventories              mi;
};

class CRefGroupTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CRefGroupTreeView(QWidget *parent = 0);
    virtual ~CRefGroupTreeView();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

private:
    friend QMenu *CAccountingOperation::getContextMenu(void) const;

    QMenu  *menu;
};

class CInventoriesTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CInventoriesTreeView (QWidget *parent = 0);
    virtual  ~CInventoriesTreeView();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    friend QMenu *CAccountingOperation::getContextMenu(void) const;
    void   draging();

    QPoint startPosition;
    QMenu  *menu;
};

#endif // CACCOUNTING_H

#pragma once

#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "source/core/dictionarycore.h"
#include "source/core/logisticnamespace.h"
#include "source/dialog/newitemdialog.h"
#include "source/dialog/inventoriesdialog.h"
//#include "ui_inventoriesdialog.h"
#include "source/dialog/refgrpdialog.h"
//#include "ui_refgrpdialog.h"

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
#define  INVENTORIES_MODEL_COLUMN_COUNT  4
#define  GROUP_TYPE_ACCOUNTING_OPERATION 114
#define  SKIP 0

namespace Ui {
class AccountingOperation;
}

class LineEditAccountingOperation;
class TreeViewInventories;

typedef struct {
    int idInventories;
    int idRefGroup;
    QString nameInventories;
    QString nameRefGroup;
} MoveRecordInventories;

class AccountingOperation : public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)

public:
    explicit AccountingOperation(QWidget *parent = 0);
    virtual ~AccountingOperation();

    bool actualRecords;

    QToolButton *getClearButton(void) const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelReferenceGroup;
    QStandardItemModel    *modelInventories;
    QSortFilterProxyModel *modelProxy;
    QItemSelectionModel   *modelSelectionReferenceGroup;
    QItemSelectionModel   *modelSelectionInventories;

    void fillingRefGroupModel (QSqlQuery &stored);
    void fillingInventoriesModel (QSqlQuery &stored);
    bool keyboardCapture (QKeyEvent *event)   const;
    QMultiMap<int, QString> setTreeData (QSqlQuery &stored, QStandardItem *p);
    QMultiMap<QString, QString> namePerIndex (QSqlQuery &stored, QMultiMap<int, QString> &m);
    void setTreeValue (QSqlQuery &stored, QStandardItem *p, int row);

Q_SIGNALS:
    void filterWildcard       (QString);
    void pushSelectRecordData (void);

private slots:
    void slotSelectRecords            (bool actual);
    void slotSelectRecordsRefGroup    (bool actual);
    void slotSelectRecordsInventories (bool actual);
    void slotClearSearchToItem        (void);
    void slotDataChanged              (const QModelIndex &index);
    void slotCutRecords               (void);
    void slotCopyRecords              (void);
    void slotRefreshRecordsRefGroup   (void);
    void slotRefreshRecordsInventory  (void);
    void slotPasteRecords             (void);
    void slotDeleteRecords            (void);
    void slotRefreshRecords           (void);
    void slotCreateEditDialog         (int r);
    void slotPushSelectRecordData     (void);
    void slotInsertOrUpdateRecords    (void);

private:
    Ui::AccountingOperation     *ui;

    QStandardItem               *rootItem;
    QTreeView                   *treeViewRefGroup;
    TreeViewInventories         *treeViewInventories;
    LineEditAccountingOperation *lineEditSearchToItem;

    RefGrpDialog                *editDialogRefGroup;
    InventoriesDialog           *editDialogInventories;

    QWidget                     *lastFocusedWidget;
    NewItemDialog               *newItem;

    RecordActionDatabase::Enum   rad;
    MoveRecordInventories        mr;
};

class TreeViewInventories : public QTreeView
{
    Q_OBJECT

public:
    explicit TreeViewInventories (QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void   draging();
    QPoint startPosition;
};

class LineEditAccountingOperation: public QLineEdit
{
    Q_OBJECT

public:
    LineEditAccountingOperation(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void slotUpdateCloseButton(const QString &text);

private:
    friend QToolButton *AccountingOperation::getClearButton(void) const;

    QToolButton *clearButton;
    QToolButton *searchButton;
};


#endif // ACCOUNTING_H

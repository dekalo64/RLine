#pragma once

#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"

#include "source/crm_dialog/dlg_supplier_g.h"
#include "source/crm_dialog/dlg_supplier.h"

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

#define  COUNTRY_MODEL_COLUMN_COUNT   4
#define  SUPPLIERS_MODEL_COLUMN_COUNT 3
#define  GROUP_TYPE_SUPPLIERS 104
#define  SKIP 0

namespace Ui {
class CSuppliers;
}

class CSCountryTreeView;
class CSuppliersTreeView;

typedef struct {
    int codeSuppliers;
    int codeCountry;
    QString nameSuppliers;
    QString nameCountry;
} MoveSupplier;

class CSuppliers: public QWidget, public CDictionaryCore
{
    Q_OBJECT

public:
    explicit CSuppliers(QWidget *parent = 0);
    virtual ~CSuppliers();

    bool actualRecords;

    QMenu *getContextMenu(void) const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelCountry;
    QStandardItemModel    *modelSuppliers;
    QItemSelectionModel   *modelSelectionCountry;
    QItemSelectionModel   *modelSelectionSuppliers;

    void fillCountryModel (QSqlQuery &stored, const QModelIndex &index);
    void fillSuppliersModel (QSqlQuery &stored);
    bool fillListSelectedRecord (QList<QVariant> &param);

private slots:
    void slotFillGroup     (const QModelIndex &index);
    void slotFillSuppliers (const QModelIndex &index);
    void slotClearGroup    (const QModelIndex &index);

    void slotActualRecords (const bool &actual);
    void slotFindSuppliers          (const QString &text);
    void slotCutRecords             (void);
    void slotCopyRecords            (void);
    void slotPasteRecords           (void);
    void slotDeleteRecords          (void);
    void slotRefreshRecords         (void);
    void slotRefreshRecordsCountry  (void);
    void slotRefreshRecordsSuppliers(void);
    void slotCreateEditDialog       (const QString &action);
    void slotCreateEditDialog       (void);
    void slotInsertOrUpdateRecords  (const QList<QString> &param);

private:
    Ui::CSuppliers     *ui;

    QStandardItem      *root;

    CSCountryTreeView  *treeCountry;
    CSuppliersTreeView *treeSuppliers;
    CFilter            *filter;

    CSupplier_gDialog  *countryDialog;
    CSupplierDialog    *supplierDialog;

    QWidget            *focusedWidget;

    Action              act ;
    MoveSupplier        ms;
};

class CSCountryTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CSCountryTreeView(QWidget *parent = 0);
    virtual ~CSCountryTreeView();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

private:
    friend QMenu *CSuppliers::getContextMenu(void) const;

    QMenu  *menu;
};

class CSuppliersTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CSuppliersTreeView (QWidget *parent = 0);
    virtual ~CSuppliersTreeView();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    friend QMenu *CSuppliers::getContextMenu(void) const;
    void   draging();

    QPoint startPosition;
    QMenu  *menu;
};

#endif // SUPPLIERS_H

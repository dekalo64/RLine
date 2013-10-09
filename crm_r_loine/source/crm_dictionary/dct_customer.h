#pragma once

#ifndef CCUSTOMER_H
#define CCUSTOMER_H

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"
#include "source/crm_dialog/dlg_additem.h"
#include "source/crm_dialog/dlg_customer.h"
#include "source/crm_dialog/dlg_discount.h"
#include "source/crm_dialog/dlg_customer_g.h"

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
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>

#define  FACES_MODEL_COLUMN_COUNT    4
#define  PARTNER_MODEL_COLUMN_COUNT  6
#define  HUMAN_MODEL_COLUMN_COUNT    5
#define  GROUP_TYPE_CUSTOMER 103
#define  SKIP 0

namespace Ui {
class CCustomer;
}

class CCustomerTreeView;

typedef struct {
    int idCustomer;
    QString nameCustomer;
} MoveCustomer;

class CCustomer : public QWidget, public CDictionaryCore
{
    Q_OBJECT

public:
    explicit CCustomer(QWidget *parent = 0);
    virtual ~CCustomer();

    bool actualRecords;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelFaces;
    QStandardItemModel    *modelPartner;
    QStandardItemModel    *modelHuman;
    QItemSelectionModel   *modelSelectionFaces;
    QItemSelectionModel   *modelSelectionPartner;
    QItemSelectionModel   *modelSelectionHuman;

    void fillFacesModel  (const QModelIndex &index, QSqlQuery &stored);
    void fillPartnerModel(QSqlQuery &stored);
    void columnHidden    (QTreeView *view, QStandardItemModel *model, const QVector<int> &vector);
    bool fillFormSelectedRecord (void);

private slots:
    void slotFillGroup  (const QModelIndex &index);
    void slotFillPartner(const QModelIndex &index);
//    void slotFillHuman  (const QModelIndex &index);
    void slotClearGroup (const QModelIndex &index);

//    void slotActualRecords (const bool &actual);
//    void slotFindCities             (const QString &text);
    void slotCutRecords             (void);
    void slotCopyRecords            (void);
    void slotPasteRecords           (void);
    void slotDeleteRecords          (void);
//    void slotRefreshRecords         (void);
//    void slotRefreshRecordsCountry  (void);
//    void slotRefreshRecordsCity     (void);
    void slotCreateEditDialog       (const QString &action);
    void slotShowEditDialog         (void);
    void slotInsertOrUpdateRecords  (void);

private:
    Ui::CCustomer     *ui;

    QStandardItem     *root;

    QTreeView         *treeFaces;
    CCustomerTreeView *treePartner;
    QTextEdit         *textEditPartnerComment;
    CCustomerTreeView *treeHuman;
    QTextEdit         *textEditHumanComment;

    CCustomerDialog   *customerDialog;
    CCustomer_gDialog *customer_gDialog;
    CDiscountDialog   *discountDialog;
    CAddItem          *addItem;

    CFilter           *filter;
    QWidget           *focusedWidget;

    Action             act;
    MoveCustomer       mc;
};

class CCustomerTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CCustomerTreeView (QWidget *parent = 0);

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

#endif // CCUSTOMER_H

#pragma once

#ifndef CCUSTOMER_H
#define CCUSTOMER_H

#include "source/core/dictionarycore.h"
#include "source/core/logisticnamespace.h"
#include "source/dialog/newitemdialog.h"

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

class CCustomer : public QWidget, public CDictionaryCore
{
    Q_OBJECT

public:
    explicit CCustomer(QWidget *parent = 0);
    virtual ~CCustomer();

    bool actualRecords;

protected:
  //  bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelFaces;
    QStandardItemModel    *modelPartner;
    QStandardItemModel    *modelHuman;
    QItemSelectionModel   *modelSelectionFaces;
    QItemSelectionModel   *modelSelectionPartner;
    QItemSelectionModel   *modelSelectionHuman;

    void fillFacesModel  (const QModelIndex &index, QSqlQuery &stored);
    void fillPartnerModel(QSqlQuery &stored);
    void columnHidden    (const QVector<int> &vector);

private slots:
    void slotFillGroup  (const QModelIndex &index);
    void slotFillPartner(const QModelIndex &index);
    void slotClearGroup (const QModelIndex &index);

private:
    Ui::CCustomer     *ui;

    QStandardItem     *root;

    QTreeView         *treeViewFaces;
    CCustomerTreeView *treeViewPartner;
    QTextEdit         *textEditPartnerComment;
    CCustomerTreeView *treeViewHuman;
    QTextEdit         *textEditHumanComment;

    CFilter           *filter;
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

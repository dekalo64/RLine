#pragma once

#ifndef DICTIONARYTEMPLATE_H
#define DICTIONARYTEMPLATE_H

#include "ui_dct_cppsst.h"

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"
#include "source/crm_dialog/dlg_cppsst.h"

#include <QtGui/QStandardItemModel>
#include <QtGui/QWidget>
#include <QtGui/QTreeView>
#include <QtGui/QToolButton>
#include <QtGui/QStyle>
#include <QtGui/QLineEdit>

class CTreeViewCppsst;

class CCppsst: public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)

public:
    explicit CCppsst(QWidget *parent = 0);
    virtual ~CCppsst();

    void columnHidden  (QTreeView *view, QStandardItemModel *model, const QVector<int> &vector);

protected slots:
    virtual void slotCreateEditDialog(const int &r) = 0; // abstract method

public:
    Ui::CCppsst                 *ui;
    CTreeViewCppsst             *treeViewCppsst;
    CCppsstDialog               *cppsstDialog;
    CFilter                     *filter;

    RecordActionDatabase::Enum   rad;
};

class CTreeViewCppsst : public QTreeView
{
    Q_OBJECT

public:
    CTreeViewCppsst(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void draging();
    QPoint startPosition;
};

#endif // DICTIONARYTEMPLATE_H

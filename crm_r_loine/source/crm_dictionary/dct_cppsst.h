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

public:
    explicit CCppsst(QWidget *parent = 0);
    virtual ~CCppsst();

    QMenu *getContextMenu(void) const;

protected slots:
    virtual void slotCreateEditDialog(const QString &action) = 0; // abstract method

Q_SIGNALS:
    void enabledComboBox(const bool &);

public:
    Ui::CCppsst     *ui;

    CTreeViewCppsst *treeCppsst;
    CCppsstDialog   *cppsstDialog;
    CFilter         *filter;

    Action           act;
};

class CTreeViewCppsst : public QTreeView
{
    Q_OBJECT

public:
    explicit CTreeViewCppsst(QWidget *parent = 0);
    virtual  ~CTreeViewCppsst();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    friend QMenu *CCppsst::getContextMenu(void) const;
    void draging();

    QPoint startPosition;
    QMenu  *menu;
};

#endif // DICTIONARYTEMPLATE_H

#pragma once

#ifndef DICTIONARYTEMPLATE_H
#define DICTIONARYTEMPLATE_H

#include "ui_dct_cppsst.h"

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"
#include "source/crm_dialog/dlg_cppsst.h"

#include <QtGui/QWidget>
#include <QtGui/QTreeView>
#include <QtGui/QToolButton>
#include <QtGui/QStyle>
#include <QtGui/QLineEdit>

class LineEdit;
class TreeView;

class DictionaryTemplate: public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)

public:
    explicit DictionaryTemplate(QWidget *parent = 0);
    virtual ~DictionaryTemplate();

    QToolButton *getClearButton(void) const;

Q_SIGNALS:
    void pushSelectRecordData(void);
    void selectionModelIndex (const QModelIndex &index);

public slots:
    void slotCreateEditDialog(int r);
    void slotClearSearchToItem();

public:
    Ui::DictionaryTemplate      *ui;
    TreeView                    *treeView;
    DictionaryDialog            *dictionaryDialog;
    LineEdit                    *lineEditSearchToItem;
    RecordActionDatabase::Enum   m_rad;
    bool                         m_selectedItem;
};

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget *parent = 0);

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

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void slotUpdateCloseButton(const QString &text);

private:
    friend QToolButton *DictionaryTemplate::getClearButton(void) const;

    QToolButton *clearButton;
    QToolButton *searchButton;
};

#endif // DICTIONARYTEMPLATE_H

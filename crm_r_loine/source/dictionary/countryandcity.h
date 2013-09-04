#pragma once

#ifndef COUNTRYANDCITY_H
#define COUNTRYANDCITY_H

#include "source/dialog/citydialog.h"
#include "ui_citydialog.h"
#include "source/dialog/countrydialog.h"
#include "ui_countrydialog.h"
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
#include <QtGui/QToolButton>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>

namespace Ui {
class CountryAndCity;
}

class LineEditCountryAndCity;
class TreeViewCity;

typedef struct {
    int idCity;
    int idCountry;
    QString nameCity;
    QString nameCountry;
}MoveItem;

class CountryAndCity : public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)
    
public:
    explicit CountryAndCity(QWidget *parent = 0);
    virtual ~CountryAndCity();

    bool actualRecords;

    QToolButton *getClearButton(void) const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *m_countryModel;
    QStandardItemModel    *m_cityModel;
    QSortFilterProxyModel *m_proxyModel;
    QItemSelectionModel   *m_selectionCountryModel;
    QItemSelectionModel   *m_selectionCityModel;

    void fillingCountryModel  (QSqlQuery &stored);
    void fillingCityModel     (QSqlQuery &stored);
    bool keyboardCapture      (QKeyEvent *event)   const;
    QMultiMap<int, QString> setTreeData(QSqlQuery &stored, QStandardItem *p);
    QMultiMap<QString, QString> namePerIndex(QSqlQuery &stored, QMultiMap<int, QString> &m);
    void setTreeValue(QSqlQuery &stored, QStandardItem *p, int row);

Q_SIGNALS:
    void filterWildcard       (QString);
    void pushSelectRecordData (void);

private slots:
    void slotSelectRecords        (bool actual);
    void slotSelectRecordsCountry (bool actual);
    void slotSelectRecordsCity    (bool actual);
    void slotCutRecords           (void);
    void slotCopyRecords          (void);
    void slotPasteRecords         (void);
    void slotDeleteRecords        (void);
    void slotRefreshRecords       (void);
    void slotRefreshRecordsCountry(void);
    void slotRefreshRecordsCity   (void);
    void slotDataChanged          (const QModelIndex &index);
    void slotCreateEditDialog     (int r);
    void slotPushSelectRecordData (void);
    void slotInsertOrUpdateRecords(void);
    void slotClearSearchToItem    (void);

private:
    Ui::CountryAndCity        *ui;

    QStandardItem             *rootItem;
    QTreeView                 *treeViewCountry;
    TreeViewCity              *treeViewCity;
    LineEditCountryAndCity    *lineEditSearchToItem;

    CityDialog                *editDialogCity;
    CountryDialog             *editDialogCountry;

    QWidget                   *lastFocusedWidget;
    NewItemDialog             *newItem;

    RecordActionDatabase::Enum m_rad;
    MoveItem                   mc;
};

class TreeViewCity : public QTreeView
{
    Q_OBJECT

public:
    explicit TreeViewCity (QWidget *parent = 0);

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

class LineEditCountryAndCity : public QLineEdit
{
    Q_OBJECT

public:
    LineEditCountryAndCity(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void slotUpdateCloseButton(const QString &text);

private:
    friend QToolButton *CountryAndCity::getClearButton(void) const;

    QToolButton *clearButton;
    QToolButton *searchButton;
};

#endif // COUNTRYANDCITY_H

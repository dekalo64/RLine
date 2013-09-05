#pragma once

#ifndef COUNTRYCITY_H
#define COUNTRYCITY_H

#include "ui_dlg_city.h"
#include "ui_dlg_country.h"

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"
#include "source/crm_dialog/dlg_city.h"
#include "source/crm_dialog/dlg_country.h"
#include "source/crm_dialog/dlg_additem.h"

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

#define  COUNTRY_MODEL_COLUMN_COUNT 4
#define  CITY_MODEL_COLUMN_COUNT    5
#define  SKIP 0

namespace Ui {
class CCountryCity;
}

class CCityTreeView;

typedef struct {
    int idCity;
    int idCountry;
    QString nameCity;
    QString nameCountry;
} MoveItem;

class CCountryCity : public QWidget, public CDictionaryCore
{
    Q_OBJECT
    Q_ENUMS(RecordActionDatabase::Enum)
    
public:
    explicit CCountryCity(QWidget *parent = 0);
    virtual ~CCountryCity();

    bool actualRecords;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelCountry;
    QStandardItemModel    *modelCity;
    QItemSelectionModel   *modelSelectionCountry;
    QItemSelectionModel   *modelSelectionCity;

    void fillCountryModel (const QModelIndex &index, QSqlQuery &stored);
    void fillCityModel (QSqlQuery &stored);
    void columnHidden  (QTreeView *view, QStandardItemModel *model, const QVector<int> &vector);

Q_SIGNALS:
    void fillFormSelectedRecord (void);

private slots:
    void slotFillGroup  (const QModelIndex &index);
    void slotFillCities (const QModelIndex &index);
    void slotClearGroup (const QModelIndex &index);

    void slotCutRecords             (void);
    void slotCopyRecords            (void);
    void slotPasteRecords           (void);
    void slotDeleteRecords          (void);
    void slotRefreshRecords         (void);
    void slotRefreshRecordsCountry  (void);
    void slotRefreshRecordsCity     (void);
    void slotCreateEditDialog       (int r);
    void slotFillFormSelectedRecord (void);
    void slotInsertOrUpdateRecords  (void);

private:
    Ui::CCountryCity *ui;

    QStandardItem    *root;

    QTreeView        *treeViewCountry;
    CCityTreeView    *treeViewCity;
    CFilter          *filter;

    CityDialog       *cityDialog;
    CountryDialog    *countryDialog;

    QWidget          *focusedWidget;
    AddItem          *addItem;

    RecordActionDatabase::Enum rad;
    MoveItem                   mc;
};

class CCityTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CCityTreeView (QWidget *parent = 0);

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

#endif // COUNTRYCITY_H

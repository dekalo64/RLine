#pragma once

#ifndef COUNTRYCITY_H
#define COUNTRYCITY_H

#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticnamespace.h"

#include "source/crm_dialog/dlg_city.h"
#include "source/crm_dialog/dlg_country.h"

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

class CCountryTreeView;
class CCityTreeView;

typedef struct {
    int codeCity;
    int codeCountry;
    QString nameCity;
    QString nameCountry;
} MoveCity;

class CCountryCity : public QWidget, public CDictionaryCore
{
    Q_OBJECT

public:
    explicit CCountryCity(QWidget *parent = 0);
    virtual ~CCountryCity();

    bool actualRecords;

    QMenu *getContextMenu(void) const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel    *modelCountry;
    QStandardItemModel    *modelCity;
    QItemSelectionModel   *modelSelectionCountry;
    QItemSelectionModel   *modelSelectionCity;

    void fillCountryModel (QSqlQuery &stored, const QModelIndex &index);
    void fillCityModel (QSqlQuery &stored);
    bool fillListSelectedRecord(QList<QString> &param);

private slots:
    void slotFillGroup  (const QModelIndex &index);
    void slotFillCities (const QModelIndex &index);
    void slotClearGroup (const QModelIndex &index);

    void slotActualRecords (const bool &actual);
    void slotFindCities             (const QString &text);
    void slotCutRecords             (void);
    void slotCopyRecords            (void);
    void slotPasteRecords           (void);
    void slotDeleteRecords          (void);
    void slotRefreshRecords         (void);
    void slotRefreshRecordsCountry  (void);
    void slotRefreshRecordsCity     (void);
    void slotCreateEditDialog       (const QString &action);
    void slotCreateEditDialog       (void);
    void slotInsertOrUpdateRecords  (const QList<QString> &param);

private:
    Ui::CCountryCity *ui;

    QStandardItem    *root;

    CCountryTreeView *treeCountry;
    CCityTreeView    *treeCity;
    CFilter          *filter;

    CCountryDialog   *countryDialog;
    CCityDialog      *cityDialog;


    QWidget          *focusedWidget;

    Action            act;
    MoveCity          mc;
};

class CCountryTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CCountryTreeView(QWidget *parent = 0);
    virtual ~CCountryTreeView();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

private:
    friend QMenu *CCountryCity::getContextMenu(void) const;

    QMenu  *menu;
};

class CCityTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CCityTreeView (QWidget *parent = 0);
    virtual ~CCityTreeView();

private slots:
    void slotCustomContextMenuRequested(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    friend QMenu *CCountryCity::getContextMenu(void) const;
    void   draging();

    QPoint startPosition;
    QMenu  *menu;
};

#endif // COUNTRYCITY_H

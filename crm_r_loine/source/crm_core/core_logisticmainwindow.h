#pragma once

#ifndef LOGISTICMAINWINDOW_H
#define LOGISTICMAINWINDOW_H

#include "source/crm_core/core_mdiwindow.h"
#include "source/crm_core/core_logisticapplication.h"

#include <QtCore/QSignalMapper>
#include <QtCore/QDateTime>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QIcon>
#include <QtGui/qevent.h>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QLayout>
#include <QtGui/QShortcut>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDockWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QCommandLinkButton>

#define __ICON__WIDTH  18
#define __ICON__HEIGHT 18

#define IS_VALID_PTR(ptr) ((ptr != (void*)0))

QT_BEGIN_NAMESPACE
class MdiWindow;
class QMenu;
class QAction;
class QActionGroupe;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE


class LogisticMainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit LogisticMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~LogisticMainWindow();
    QSize sizeHint() const;

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void slotNewDocument();
    void slotOpenDocument();
    void slotSaveDocument();
    void slotPrintDocument();
    void slotCutDocument();
    void slotCopyDocument();
    void slotPasteDocument();
    void slotDeleteDocument();
//    void slotSearchDocument();
//    void slotSearchNextDocument();
//    void slotSearchPrevDocument();
    void slotRefreshDocument();
    void slotOwnerDocument();
    void slotActualDocumentsView();
    void slotTreeDocumentsView();

    void slotRelationsFolder();
    void slotCalendarFolder() const;
    void slotReminderFolder();

    void slotCustomerDictionary();
    void slotSuppliersDictionary();
    void slotProdusersDictionary();
    void slotContactsDictionary();
    void slotPositionsDictionary();
    void slotTaskTypesDictionary();
    void slotContractorTypesDictionary();
    void slotStatusDictionary();
    void slotPrioritiesDictionary();
    void slotCountryAndCityDictionary();

    void slotGeometryChangeRequested(const QRect &geometry);

    void slotSwitchLayoutDirection();
    void slotSetActiveSubWindow(QWidget *window);

    void slotAccountingAdditionally();

private:
    void connectionWindowShow();
    MdiWindow      *activeMdiWindow();
    QMdiSubWindow  *findMdiWindow(const QString &title);
    void showMdiWindow(QWidget *widget, const QString &title, const QIcon &icon);
    void updateWindowTitle(const QString &title = QString());
    void execAction(const QString &stored, const QVariant &arg);

    friend class MdiWindow;

public:
    QMdiArea *m_mdiArea;
    void setupDockWindow(bool visible);
    void setupMenu(bool visible);
    void setupToolBar(bool visible);
    void setupStatusBar(bool visible);

private:
    QSignalMapper *m_windowMapper;
    QMenu         *m_windowMenu;
    QToolBar      *m_actionBar;
};

#endif // LOGISTICMAINWINDOW_H

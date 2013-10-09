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


class CLogisticMainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit CLogisticMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~CLogisticMainWindow();

    QSize sizeHint() const;

    static CLogisticMainWindow *instance(){
        if (!self){
             self = new CLogisticMainWindow();
        }
        return static_cast<CLogisticMainWindow *>(self);
    }

protected:
    void closeEvent(QCloseEvent *);

    static CLogisticMainWindow *self;

private slots:
    void slotNewDocument();
    void slotOpenDocument();
    void slotPrintDocument();
    void slotCutDocument();
    void slotCopyDocument();
    void slotPasteDocument();
    void slotDeleteDocument();
    void slotRefreshDocument();
    void slotOwnerDocument();
    void slotActualDocumentsView();
    void slotTreeDocumentsView();

    void slotRelationsFolder();
    void slotCalendarFolder() const;
    void slotReminderFolder();

    void slotCustomerDictionary();
    void slotSuppliersDictionary();
    void slotContactsDictionary();
    void slotPositionsDictionary();
    void slotTaskTypesDictionary();
    void slotContractorTypesDictionary();
    void slotStatusDictionary();
    void slotPrioritiesDictionary();

    void slotGeometryChangeRequested(const QRect &geometry);

    void slotSwitchLayoutDirection();
    void slotSetActiveSubWindow(QWidget *window);

public slots:
    void slotCountryCityDictionary();
    void slotProdusersDictionary();

private:
    void connectionWindowShow(QWidget *parent = 0);

    MdiWindow      *activeMdiWindow();
    QMdiSubWindow  *findMdiWindow(const QString &title);

    void showMdiWindow(QWidget *widget, const QString &title, const QIcon &icon);
    void updateWindowTitle(const QString &title = QString());
    void execAction(const QString &stored, const QVariant &arg = QVariant());

public:
    QMdiArea *mdiArea;

    friend class MdiWindow;

    void setupDockWindow (const bool &visible);
    void setupMenu       (const bool &visible);
    void setupToolBar    (const bool &visible);
    void setupStatusBar  (const bool &visible);

private:
    QSignalMapper *windowMapper;
    QMenu         *windowMenu;
    QToolBar      *actionBar;
};

#endif // LOGISTICMAINWINDOW_H

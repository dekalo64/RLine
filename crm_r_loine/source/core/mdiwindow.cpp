#include "source/core/mdiwindow.h"
#include "source/core/logisticapplication.h"
#include "source/core/logisticmainwindow.h"

MdiWindow::MdiWindow(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void MdiWindow::closeEvent(QCloseEvent * /* *event */)
{
    if (LogisticApplication::instance()->positions != nullptr &&
       (LogisticApplication::instance()->mainWnd->activeMdiWindow()->accessibleDescription() ==
        LogisticApplication::instance()->positions->metaObject()->className())) {
                delete LogisticApplication::instance()->positions;
                LogisticApplication::instance()->positions = nullptr;
        return;
    }

    if (LogisticApplication::instance()->contractortype != nullptr &&
       (LogisticApplication::instance()->mainWnd->activeMdiWindow()->accessibleDescription() ==
        LogisticApplication::instance()->contractortype->metaObject()->className())) {
                delete LogisticApplication::instance()->contractortype;
                LogisticApplication::instance()->contractortype = nullptr;
        return;
    }

    if (LogisticApplication::instance()->tasktype &&
       (LogisticApplication::instance()->mainWnd->activeMdiWindow()->accessibleDescription() ==
        LogisticApplication::instance()->tasktype->metaObject()->className())) {
                delete LogisticApplication::instance()->tasktype;
                LogisticApplication::instance()->tasktype = nullptr;
        return;
    }

    if (LogisticApplication::instance()->priorities &&
       (LogisticApplication::instance()->mainWnd->activeMdiWindow()->accessibleDescription() ==
        LogisticApplication::instance()->priorities->metaObject()->className())) {
                delete LogisticApplication::instance()->priorities;
                LogisticApplication::instance()->priorities = nullptr;
        return;
    }

    if (LogisticApplication::instance()->status &&
       (LogisticApplication::instance()->mainWnd->activeMdiWindow()->accessibleDescription() ==
        LogisticApplication::instance()->status->metaObject()->className())) {
                delete LogisticApplication::instance()->status;
                LogisticApplication::instance()->status = nullptr;
        return;
    }

    if (LogisticApplication::instance()->countryandcity &&
       (LogisticApplication::instance()->mainWnd->activeMdiWindow()->accessibleDescription() ==
        LogisticApplication::instance()->countryandcity->metaObject()->className())) {
                delete LogisticApplication::instance()->countryandcity;
                LogisticApplication::instance()->countryandcity = nullptr;
        return;
    }
}

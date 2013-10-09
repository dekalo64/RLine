#include "source/crm_core/core_mdiwindow.h"
#include "source/crm_core/core_logisticapplication.h"
#include "source/crm_core/core_logisticmainwindow.h"

MdiWindow::MdiWindow(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void MdiWindow::closeEvent(QCloseEvent * /* *event */)
{
    if (CLogisticApplication::instance()->position != nullptr &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->position->metaObject()->className())) {
                delete CLogisticApplication::instance()->position;
                CLogisticApplication::instance()->position = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->contractortype != nullptr &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->contractortype->metaObject()->className())) {
                delete CLogisticApplication::instance()->contractortype;
                CLogisticApplication::instance()->contractortype = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->tasktype &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->tasktype->metaObject()->className())) {
                delete CLogisticApplication::instance()->tasktype;
                CLogisticApplication::instance()->tasktype = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->priority &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->priority->metaObject()->className())) {
                delete CLogisticApplication::instance()->priority;
                CLogisticApplication::instance()->priority = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->status &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->status->metaObject()->className())) {
                delete CLogisticApplication::instance()->status;
                CLogisticApplication::instance()->status = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->countrycity &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->countrycity->metaObject()->className())) {
                delete CLogisticApplication::instance()->countrycity;
                CLogisticApplication::instance()->countrycity = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->customer &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->customer->metaObject()->className())) {
                delete CLogisticApplication::instance()->customer;
                CLogisticApplication::instance()->customer = nullptr;
        return;
    }

    if (CLogisticApplication::instance()->supplier &&
       (CLogisticMainWindow::instance()->activeMdiWindow()->accessibleDescription() ==
        CLogisticApplication::instance()->supplier->metaObject()->className())) {
                delete CLogisticApplication::instance()->supplier;
                CLogisticApplication::instance()->supplier = nullptr;
        return;
    }
}

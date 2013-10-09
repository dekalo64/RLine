#include "source/crm_core/core_logisticapplication.h"
#include "source/crm_core/core_logisticmainwindow.h"

int main(int argc, char *argv[])
{
    CLogisticApplication application(argc, argv);
    CLogisticMainWindow::instance();

    return application.exec();
}

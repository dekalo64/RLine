#include "source/crm_core/core_logisticapplication.h"

int main(int argc, char *argv[])
{
    LogisticApplication application(argc, argv);

    application.mainWindow();
    return application.exec();
}

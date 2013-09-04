#include "logisticapplication.h"

int main(int argc, char *argv[])
{
    LogisticApplication application(argc, argv);

    application.mainWindow();
    return application.exec();
}

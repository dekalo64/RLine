#include "source/crm_core/core_logisticapplication.h"
#include "source/crm_core/core_logisticmainwindow.h"

//CLogisticMainWindow *CLogisticApplication::mainWnd      = nullptr;

QSettings           *CLogisticApplication::settings     = nullptr;
ConnectionDialog    *CLogisticApplication::cntnDialog   = nullptr;

CPositions      *CLogisticApplication::position        = nullptr;
CTaskType       *CLogisticApplication::tasktype         = nullptr;
CContractorType *CLogisticApplication::contractortype   = nullptr;
CStatus         *CLogisticApplication::status           = nullptr;
CPriorities     *CLogisticApplication::priority       = nullptr;
CCountryCity    *CLogisticApplication::countrycity      = nullptr;
CSuppliers      *CLogisticApplication::supplier        = nullptr;
CCustomer       *CLogisticApplication::customer         = nullptr;

CLogisticApplication::CLogisticApplication(int &argc, char **argv)
    :QApplication(argc, argv)
{
    QTextCodec::setCodecForLocale  (QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr      (QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    QApplication::setQuitOnLastWindowClosed(true);

    settings = new QSettings(qApp->applicationDirPath() + "/connect.ini", QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("utf-8"));
}

CLogisticApplication::~CLogisticApplication()
{
    if (database.isOpen()) { database.close(); }

    if (IS_VALID_PTR(position))        {  position       = nullptr; }
    if (IS_VALID_PTR(tasktype))        {  tasktype       = nullptr; }
    if (IS_VALID_PTR(contractortype))  {  contractortype = nullptr; }
    if (IS_VALID_PTR(status))          {  status         = nullptr; }
    if (IS_VALID_PTR(priority))        {  priority       = nullptr; }
    if (IS_VALID_PTR(countrycity))     {  countrycity    = nullptr; }
    if (IS_VALID_PTR(supplier))        {  supplier       = nullptr; }
    if (IS_VALID_PTR(customer))        {  customer       = nullptr; }

    if (IS_VALID_PTR(cntnDialog))      {  cntnDialog     = nullptr; }

    if (IS_VALID_PTR(settings))        {  delete settings; settings = nullptr; }
}

CLogisticApplication *CLogisticApplication::instance()
{
    return (static_cast<CLogisticApplication *>(QCoreApplication::instance()));
}

ConnectionDialog *CLogisticApplication::connectionDialog(QWidget *parent)
{
    if (!cntnDialog){
         cntnDialog = new ConnectionDialog(parent);
    }

    if (cntnDialog){
        connect(cntnDialog, SIGNAL(sendUserInformation(QString,QString)), CLogisticApplication::instance(), SLOT(slotAddConnection(QString,QString)));
        connect(CLogisticApplication::instance(), SIGNAL(successConnection()), cntnDialog, SLOT(slotSuccessConnection()));
        connect(cntnDialog, SIGNAL(setVisibleComponent(bool)), CLogisticApplication::instance(), SLOT(slotVisibleComponent(bool)));
    }
    setStyleWidget(cntnDialog, "connection.qss");

    return cntnDialog;
}

CPositions *CLogisticApplication::createPosition()
{
    position == nullptr ? position = new CPositions() : position = nullptr;

    if (!position){
         position = new CPositions();
         position->setObjectName ("Position");
    }

    return position;
}

CTaskType *CLogisticApplication::createTaskType()
{
    tasktype == nullptr ? tasktype = new CTaskType() : tasktype = nullptr;

    if (!tasktype){
         tasktype = new CTaskType();
         tasktype->setObjectName ("TaskType");
    }

    return tasktype;
}

CContractorType *CLogisticApplication::createContractorType()
{
    contractortype == nullptr ? contractortype = new CContractorType() : contractortype = nullptr;

    if (!contractortype){
         contractortype = new CContractorType();
         contractortype->setObjectName ("Contractor");
    }

    return contractortype;
}

CStatus *CLogisticApplication::createStatus()
{
    status == nullptr ? status = new CStatus() : status = nullptr;

    if (!status){
         status = new CStatus();
         status->setObjectName ("Status");
    }

    return status;
}

CPriorities *CLogisticApplication::createPriorities()
{
    priority == nullptr ? priority = new CPriorities() : priority = nullptr;

    if (!priority){
         priority = new CPriorities();
         priority->setObjectName ("Priority");
    }

    return priority;
}

CCountryCity *CLogisticApplication::createCountryCity()
{
    countrycity == nullptr ? countrycity = new CCountryCity() : countrycity = nullptr;

    if (!countrycity){
         countrycity = new CCountryCity();
         countrycity->setObjectName ("CountryCity");
    }

    return countrycity;
}

// WARNING
CContacts *CLogisticApplication::createContacts()
{
    CContacts *contacts = new CContacts;
    if (!contacts){
        return nullptr;
    }
    return contacts;
}

CSuppliers *CLogisticApplication::createSappliers()
{
    supplier == nullptr ? supplier = new CSuppliers() : supplier = nullptr;

    if (!supplier){
         supplier = new CSuppliers();
         supplier->setObjectName ("Supplier");
    }

    return supplier;
}

CCustomer *CLogisticApplication::createCustomer()
{
    customer == nullptr ? customer = new CCustomer() : customer = nullptr;

    if (!customer){
         customer = new CCustomer();
         customer->setObjectName ("Customer");
    }

    return customer;
}

MdiWindow *CLogisticApplication::createMdiMindow(const QString &title, const QIcon &icon)
{
    MdiWindow *mdiWindow = new MdiWindow();
               mdiWindow->setWindowTitle(title);
               mdiWindow->setWindowIcon (icon);
    CLogisticMainWindow::instance()->mdiArea->addSubWindow(mdiWindow /* Qt::FramelessWindowHint */);

    return mdiWindow;
}

QSqlError CLogisticApplication::slotAddConnection(const QString &user, const QString &passwd)
{   
    QString templateString = QString("DRIVER={SQL Server};Server=%1;Database=%2;Uid=%3;Pwd=%4;");
    QString connectionString = QString(templateString).arg(host()).arg(dbname()).arg(user).arg(passwd);

    database = QSqlDatabase::addDatabase(driver(), "Logistic");
    database.setDatabaseName(connectionString);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    if (!database.open()) {
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
        CCommunicate::showing("Неправильно указан логин и/или пароль");
        return database.lastError();
    } else {
        emit successConnection();
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return database.lastError();
}

void CLogisticApplication::slotVisibleComponent(bool visible)
{
    setStyleWidget(CLogisticMainWindow::instance(), "application.qss");

    CLogisticMainWindow::instance()->setupToolBar   (visible);
    CLogisticMainWindow::instance()->setupStatusBar (visible);
    CLogisticMainWindow::instance()->setupMenu      (visible);
    CLogisticMainWindow::instance()->setupDockWindow(visible);
}

void CLogisticApplication::setStyleWidget(QWidget *widget, const QString &cssFile)
{
    QFile styleFile(QString(qApp->applicationDirPath() + "/data/qss/%1").arg(cssFile));
    if (styleFile.exists()){
       try{
            if (styleFile.open(QFile::ReadOnly)) {
                QString styleSheet = QLatin1String(styleFile.readAll());
                widget->setStyleSheet(styleSheet);
            }
       } catch (std::exception const &e){
            CCommunicate::showing(QString("%1").arg(e.what()));
       }
    }
}

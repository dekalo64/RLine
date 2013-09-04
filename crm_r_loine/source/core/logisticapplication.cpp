#include "logisticapplication.h"
#include "logisticmainwindow.h"
#include "ui_connectiondialog.h"

LogisticMainWindow *LogisticApplication::mainWnd          = nullptr;
QSettings          *LogisticApplication::settings         = nullptr;
ConnectionDialog   *LogisticApplication::cntnDialog       = nullptr;

Positions      *LogisticApplication::positions            = nullptr;
TaskType       *LogisticApplication::tasktype             = nullptr;
ContractorType *LogisticApplication::contractortype       = nullptr;
Status         *LogisticApplication::status               = nullptr;
Priorities     *LogisticApplication::priorities           = nullptr;
CountryAndCity *LogisticApplication::countryandcity       = nullptr;
Suppliers      *LogisticApplication::suppliers            = nullptr;
CCustomer      *LogisticApplication::customer             = nullptr;
//AccountingOperation     *LogisticApplication::accountingoperation           = nullptr;

LogisticApplication::LogisticApplication(int &argc, char **argv)
    :QApplication(argc, argv)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    QApplication::setQuitOnLastWindowClosed(true);

    settings = new QSettings(qApp->applicationDirPath() + "/connect.ini", QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("utf-8"));
}

LogisticApplication::~LogisticApplication()
{
    if (database.isOpen()) { database.close(); }

    if (IS_VALID_PTR(positions))        {                        positions         = nullptr; }
    if (IS_VALID_PTR(tasktype))         {                        tasktype          = nullptr; }
    if (IS_VALID_PTR(contractortype))   {                        contractortype    = nullptr; }
    if (IS_VALID_PTR(status))           {                        status            = nullptr; }
    if (IS_VALID_PTR(priorities))       {                        priorities        = nullptr; }
    if (IS_VALID_PTR(countryandcity))   {                        countryandcity    = nullptr; }
    if (IS_VALID_PTR(suppliers))        {                        suppliers         = nullptr; }
    if (IS_VALID_PTR(customer))         {                        customer          = nullptr; }
//    if (IS_VALID_PTR(accountingoperation))       {                        accountingoperation        = nullptr; }

    if (IS_VALID_PTR(cntnDialog)) {                              cntnDialog        = nullptr; }
    if (IS_VALID_PTR(settings))         { delete settings;       settings          = nullptr; }
    if (IS_VALID_PTR(mainWnd))          {                        mainWnd           = nullptr; }
}

LogisticApplication *LogisticApplication::instance()
{
    return (static_cast<LogisticApplication *>(QCoreApplication::instance()));
}

ConnectionDialog *LogisticApplication::connectionDialog(QWidget *parent)
{
    if (!cntnDialog){
        cntnDialog = new ConnectionDialog(parent);
    }
    if (cntnDialog){
        connect(cntnDialog, SIGNAL(sendUserInformation(QString,QString)),
                LogisticApplication::instance(), SLOT(slotAddConnection(QString,QString)));
        connect(LogisticApplication::instance(), SIGNAL(successConnection()),
                cntnDialog, SLOT(slotSuccessConnection()));
        connect(cntnDialog, SIGNAL(setVisibleComponent(bool)),
                LogisticApplication::instance(), SLOT(slotVisibleComponent(bool)));
    }
    setStyleWidget(cntnDialog, "connection.qss");
    return cntnDialog;
}

Positions *LogisticApplication::createPosition()
{
    if (positions)
        positions = nullptr;

    if (positions == nullptr){
        positions = new Positions();
    }
    return positions;
}

TaskType *LogisticApplication::createTaskType()
{
    if (tasktype)
        tasktype = nullptr;

    if (tasktype == nullptr){
        tasktype = new TaskType();
    }
    return tasktype;
}

ContractorType *LogisticApplication::createContractorType()
{
    if (contractortype)
        contractortype = nullptr;

    if (contractortype == nullptr){
        contractortype = new ContractorType();
    }
    return contractortype;
}

Status *LogisticApplication::createStatus()
{
    if (status)
        status = nullptr;

    if (status == nullptr){
        status = new Status();
    }
    return status;
}

Priorities *LogisticApplication::createPriorities()
{
    if (priorities)
        priorities = nullptr;

    if (priorities == nullptr){
        priorities = new Priorities();
    }
    return priorities;
}

CountryAndCity *LogisticApplication::createCountryAndCity()
{
    if (countryandcity)
        countryandcity = nullptr;

    if (countryandcity == nullptr){
        countryandcity = new CountryAndCity();
    }
    return countryandcity;
}

Contacts *LogisticApplication::createContacts()
{
    Contacts *contacts = new Contacts;
    if (!contacts){
        return nullptr;
    }
    return contacts;
}

Suppliers *LogisticApplication::createSappliers()
{
    if (suppliers)
        suppliers = nullptr;

    if (suppliers == nullptr){
        suppliers = new Suppliers();
    }
    return suppliers;
}

CCustomer *LogisticApplication::createCustomer()
{
    if (customer)
        customer = nullptr;

    if (customer == nullptr){
        customer = new CCustomer();
    }
    return customer;
}

//AccountingOperation *LogisticApplication::createAccountingOperation()
//{
//    if (accountingoperation)
//        accountingoperation = nullptr;

//    if (accountingoperation == nullptr){
//        accountingoperation = new AccountingOperation();
//    }
//    return accountingoperation;
//}

MdiWindow *LogisticApplication::createMdiMindow(const QString &title, const QIcon &icon)
{
    MdiWindow *m_mdiWindow = new MdiWindow();
               m_mdiWindow->setWindowTitle(title);
               m_mdiWindow->setWindowIcon(icon);
    mainWnd->m_mdiArea->addSubWindow(m_mdiWindow /* Qt::FramelessWindowHint */);

    return m_mdiWindow;
}

QSqlError LogisticApplication::slotAddConnection(const QString &user, const QString &passwd)
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
        QMessageBox::critical(0, "Ошибка", "Ошибка подключения к базе данных");
        return database.lastError();
    } else {
        emit successConnection();
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return database.lastError();
}

void LogisticApplication::slotVisibleComponent(bool visible)
{
    setStyleWidget(mainWnd, "application.qss");

    mainWnd->setupToolBar   (visible);
    mainWnd->setupStatusBar (visible);
    mainWnd->setupMenu      (visible);
    mainWnd->setupDockWindow(visible);
}

void LogisticApplication::setStyleWidget(QWidget *widget, const QString &cssFile)
{
    QFile m_styleFile(QString(qApp->applicationDirPath() + "/data/qss/%1").arg(cssFile));
    if (m_styleFile.exists()){
        if (m_styleFile.open(QFile::ReadOnly)) {
            QString m_styleSheet = QLatin1String(m_styleFile.readAll());
            widget->setStyleSheet(m_styleSheet);
        }
    }
}

QString LogisticApplication::driver() const
{
    return settings->value("database/driver").toString();
}

QString LogisticApplication::host() const
{
    return settings->value("database/host").toString();
}

QString LogisticApplication::dbname() const
{
    return settings->value("database/dbname").toString();
}

LogisticMainWindow *LogisticApplication::mainWindow()
{
    if (mainWnd == nullptr){
         mainWnd = new LogisticMainWindow();
    }
    return mainWnd;
}

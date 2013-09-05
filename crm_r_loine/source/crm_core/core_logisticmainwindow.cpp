#include "source/crm_core/core_logisticmainwindow.h"
#include "source/crm_core/core_logisticnamespace.h"

LogisticMainWindow::LogisticMainWindow(QWidget *parent, Qt::WindowFlags flags):
    QMainWindow(parent, flags)
{
    setUpdatesEnabled(true);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QRect rect = geometry();
    rect.moveCenter(QApplication::desktop()->availableGeometry().center());
    setGeometry(rect);

    showMaximized();

    m_mdiArea = new QMdiArea();
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setViewMode(QMdiArea::SubWindowView);
    m_mdiArea->setTabsClosable(true);

    m_windowMapper = new QSignalMapper(this);

    statusBar()->setVisible(false);
    statusBar()->setSizeGripEnabled(true);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_mdiArea);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    updateWindowTitle();
    connectionWindowShow();

    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));
    connect(m_windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(slotSetActiveSubWindow(QWidget*)));
}

LogisticMainWindow::~LogisticMainWindow()
{
    if (!IS_VALID_PTR(m_actionBar))          { delete m_actionBar;          m_actionBar          = nullptr; }
    if (!IS_VALID_PTR(m_windowMapper))       { delete m_windowMapper;       m_windowMapper       = nullptr; }
    if (!IS_VALID_PTR(m_mdiArea))            { delete m_mdiArea;            m_mdiArea            = nullptr; }
}

QSize LogisticMainWindow::sizeHint() const
{
    QRect desktopRect = QApplication::desktop()->screenGeometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

void LogisticMainWindow::setupMenu(bool visible)
{
    if (!visible) return;

    // Document
    QMenu *documentMenu = menuBar()->addMenu(tr("&Документ"));
    documentMenu->addAction(tr("Новый"), this, SLOT(slotNewDocument()), QKeySequence(Qt::Key_F3));
    documentMenu->addAction(tr("Открыть"), this, SLOT(slotOpenDocument()), QKeySequence(Qt::Key_F4));
    documentMenu->addAction(tr("Сохранить"), this, SLOT(slotSaveDocument()), QKeySequence(Qt::Key_F2));
    documentMenu->addSeparator();
    documentMenu->addAction(tr("Печать"), this, SLOT(slotPrintDocument()));
    documentMenu->addSeparator();
    documentMenu->addAction(tr("Переместить"), this, SLOT(slotCutDocument()), QKeySequence(Qt::Key_F6));
    documentMenu->addAction(tr("Копировать"), this, SLOT(slotCopyDocument()));
    documentMenu->addAction(tr("Вставить"),this, SLOT(slotPasteDocument()));
    documentMenu->addAction(tr("Удалить"), this, SLOT(slotDeleteDocument()), QKeySequence(Qt::Key_F8));
    documentMenu->addSeparator();
    documentMenu->addAction(tr("Найти..."), this, SLOT(slotSearchDocument()));
    documentMenu->addAction(tr("Найти следующий"), this, SLOT(slotSearchNextDocument()));
    documentMenu->addAction(tr("Найти предыдущий"), this, SLOT(slotSearchPrevDocument()));
    documentMenu->addSeparator();
    documentMenu->addAction(tr("Обновить"), this, SLOT(slotRefreshDocument()));
    documentMenu->addAction(tr("Стать владельцем"), this, SLOT(slotOwnerDocument()));
    documentMenu->addSeparator();
    documentMenu->addAction(tr("Выход"), this, SLOT(close()), QKeySequence(Qt::ALT | Qt::Key_F4));

    // Folder
    QMenu *folderMenu = menuBar()->addMenu(tr("&Папка"));
    folderMenu->addAction(tr("Взаимоотношения"), this, SLOT(slotRelationsFolder()));
    folderMenu->addAction(tr("Календать"), this, SLOT(slotCalendarFolder()));
    folderMenu->addAction(tr("Напоминания"), this, SLOT(slotReminderFolder()));

    // Dictionary
    QMenu *dictionaryMenu = menuBar()->addMenu(tr("Справочники"));
    dictionaryMenu->addAction(tr("Заказчики"), this, SLOT(slotCustomerDictionary()));
    dictionaryMenu->addAction(tr("Поставщики"), this, SLOT(slotSuppliersDictionary()));
    dictionaryMenu->addAction(tr("Производители"), this, SLOT(slotProdusersDictionary()));
    dictionaryMenu->addSeparator();
    dictionaryMenu->addAction(tr("Контакты"), this, SLOT(slotContactsDictionary()));
    dictionaryMenu->addAction(tr("Должности"), this, SLOT(slotPositionsDictionary()));
    dictionaryMenu->addSeparator();
    dictionaryMenu->addAction(tr("Типы задач"), this, SLOT(slotTaskTypesDictionary()));
    dictionaryMenu->addAction(tr("Типы контрагентов"), this, SLOT(slotContractorTypesDictionary()));
    dictionaryMenu->addAction(tr("Статусы"), this, SLOT(slotStatusDictionary()));
    dictionaryMenu->addAction(tr("Приоритеты"), this, SLOT(slotPrioritiesDictionary()));
    dictionaryMenu->addSeparator();
    dictionaryMenu->addAction(tr("Страны и города"), this, SLOT(slotCountryAndCityDictionary()));

    // View
    QMenu *viewMenu = menuBar()->addMenu(tr("&Вид"));

    QAction *m_actual = viewMenu->addAction(tr("Актуальные документы"));
    connect(m_actual, SIGNAL(triggered()), SLOT(slotActualDocumentsView()));

    viewMenu->addAction(tr("Дерево документов"), this, SLOT(slotTreeDocumentsView()), QKeySequence(Qt::Key_F9));

    // Window
    QMenu *windowMenu = menuBar()->addMenu(tr("&Окно"));

    QAction *close = windowMenu->addAction(tr("Закрыть"));
    connect(close, SIGNAL(triggered()), m_mdiArea, SLOT(closeActiveSubWindow()));

    QAction *closeAll = windowMenu->addAction(tr("Закрыть все"));
    connect(closeAll, SIGNAL(triggered()), m_mdiArea, SLOT(closeAllSubWindows()));

    windowMenu->addSeparator();

    QAction *m_tile = windowMenu->addAction(tr("Плитка"));
    connect(m_tile, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));

    QAction *m_cascade = windowMenu->addAction(tr("Каскад"));
    connect(m_cascade, SIGNAL(triggered()), m_mdiArea, SLOT(cascadeSubWindows()));

    windowMenu->addSeparator();

    QMenu *modeMenu = windowMenu->addMenu(tr("Режим отображения"));
    QAction *m_modeMdi = modeMenu->addAction(tr("Оконный"));
    connect(m_modeMdi, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));
    QAction *m_modeWindow = modeMenu->addAction(tr("Вкладки"));
    connect(m_modeWindow, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));


    windowMenu->addSeparator();

    QAction *next = windowMenu->addAction(tr("Следующее"));
    connect(next, SIGNAL(triggered()), m_mdiArea, SLOT(activateNextSubWindow()));

    QAction *previous = windowMenu->addAction(tr("Предыдущее"));
    connect(previous, SIGNAL(triggered()), m_mdiArea, SLOT(activatePreviousSubWindow()));
}

void LogisticMainWindow::setupToolBar(bool visible)
{
    if (!visible) return;

    setUnifiedTitleAndToolBarOnMac(true);

    m_actionBar = new QToolBar(tr("Действия"), this);
    m_actionBar->setAllowedAreas(Qt::TopToolBarArea | Qt::RightToolBarArea | Qt::BottomToolBarArea);
    LogisticMainWindow::addToolBar(m_actionBar);

    QAction *m_nd = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/new.png")), tr("Новый документ"));
    connect(m_nd, SIGNAL(triggered()), this, SLOT(slotNewDocument()));

    QAction *m_ne = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/edit.png")), tr("Открыть документ"));
    connect(m_ne, SIGNAL(triggered()), this, SLOT(slotOpenDocument()));

    QAction *m_save = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/save.png")), tr("Сохранить"));
    connect(m_save, SIGNAL(triggered()), this, SLOT(slotSaveDocument()));
    m_actionBar->addSeparator();

    QAction *m_refresh = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/refresh.png")), tr("Обновить"));
    connect(m_refresh, SIGNAL(triggered()), this, SLOT(slotRefreshDocument()));

    QAction *m_print = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/print.png")), tr("Печать"));
    connect(m_print, SIGNAL(triggered()), this, SLOT(slotPrintDocument()));

    m_actionBar->addSeparator();

    QAction *m_cut = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/cut.png")), tr("Вырезать"));
    connect(m_cut, SIGNAL(triggered()), SLOT(slotCutDocument()));

    QAction *m_copy = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/copy.png")), tr("Копировать"));
    connect(m_copy, SIGNAL(triggered()), this, SLOT(slotCopyDocument()));

    QAction *m_paste = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/paste.png")), tr("Вставить"));
    connect(m_paste, SIGNAL(triggered()), SLOT(slotPasteDocument()));

    QAction *m_delete = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/delete.png")), tr("Удалить"));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(slotDeleteDocument()));

    m_actionBar->addSeparator();

//    QAction *m_search = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/find.png")), tr("Найти"));
//    connect(m_search, SIGNAL(triggered()), this, SLOT(slotSearchDocument()));

//    QAction *m_nl = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/navigate-left.png")), tr("Найти предыдущий"));
//    connect(m_nl, SIGNAL(triggered()), this, SLOT(slotSearchPrevDocument()));

//    QAction *m_nr = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/navigate-right.png")), tr("Найти следующий"));
//    connect(m_nr, SIGNAL(triggered()), this, SLOT(slotSearchPrevDocument()));

    QAction *m_actualOrAll = m_actionBar->addAction(QIcon(QString("data/picture/toolbar/actual.png")), tr("Актуальные/Все"));
    connect(m_actualOrAll, SIGNAL(triggered()), SLOT(slotActualDocumentsView()));

    m_actionBar->addAction(QIcon(QString("data/picture/toolbar/treelist.png")), tr("Дерево/Список"));

    m_actionBar->setIconSize(QSize(__ICON__WIDTH, __ICON__HEIGHT));
}

void LogisticMainWindow::setupStatusBar(bool visible)
{
    if (!visible) return;

    statusBar()->setVisible(visible);

    QDate  *d = new QDate();

    QLabel *m_lblDate     = new QLabel(" Дата: <b>" + d->currentDate().toString("ddd, dd/MM/yyyy г. ") + "</b>");
    m_lblDate->setMinimumSize(m_lblDate->sizeHint());

    QLabel *m_lblDatabase = new QLabel(" База данных: <b>" + LogisticApplication::instance()->dbname() + "</b>");

    statusBar()->addWidget(m_lblDate);
    statusBar()->addWidget(m_lblDatabase);
}

void LogisticMainWindow::updateWindowTitle(const QString &title)
{
    if (title.isEmpty()) {
        setWindowTitle(tr("CRM R-Loine"));
    } else {
        setWindowTitle(tr("%1").arg(title));
    }
}

void LogisticMainWindow::execAction(const QString &stored, const QVariant &arg)
{
    QGenericArgument genericArgument;

    if (arg.isValid()){
        genericArgument = QGenericArgument(arg.typeName(), arg.data());
    }

    if (activeMdiWindow()){

        if ( LogisticApplication::instance()->positions != nullptr ){
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->positions->metaObject()->className() ) {
                QMetaObject::invokeMethod(LogisticApplication::instance()->positions,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( LogisticApplication::instance()->tasktype != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->tasktype->metaObject()->className() ) {
                QMetaObject::invokeMethod(LogisticApplication::instance()->tasktype,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( LogisticApplication::instance()->contractortype != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->contractortype->metaObject()->className() ) {
                QMetaObject::invokeMethod(LogisticApplication::instance()->contractortype,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( LogisticApplication::instance()->status != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->status->metaObject()->className() ) {
                QMetaObject::invokeMethod(LogisticApplication::instance()->status,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( LogisticApplication::instance()->priorities != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->priorities->metaObject()->className() ) {
                QMetaObject::invokeMethod(LogisticApplication::instance()->priorities,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( LogisticApplication::instance()->countryandcity != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->countryandcity->metaObject()->className() ) {
                QMetaObject::invokeMethod(LogisticApplication::instance()->countryandcity,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

//        if ( LogisticApplication::instance()->accountingoperation != nullptr ) {
//            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->accountingoperation->metaObject()->className() ) {
//                QMetaObject::invokeMethod(LogisticApplication::instance()->accountingoperation,
//                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
//            }
//        }
    }
}

void LogisticMainWindow::setupDockWindow(bool visible)
{
    if (!visible) return;

    QDockWidget *dockDictionary = new QDockWidget(tr("Справочники"));
    dockDictionary->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QFrame *frameDictionary = new QFrame(this);

    QCommandLinkButton *m_client = new QCommandLinkButton(tr("Заказчики"), tr("Справочник клиентов"), frameDictionary);
    m_client->setIcon(QIcon("data/picture/sidebar/customer.ico"));
    connect(m_client, SIGNAL(clicked()), this, SLOT(slotCustomerDictionary()));

    QCommandLinkButton *m_supplier = new QCommandLinkButton(tr("Поставщики"), tr("Справочник поставщиков"), frameDictionary);
    m_supplier->setIcon(QIcon("data/picture/sidebar/suppliers.ico"));
    connect(m_supplier, SIGNAL(clicked()), this, SLOT(slotSuppliersDictionary()));

    QCommandLinkButton *m_producer = new QCommandLinkButton(tr("Производители"), tr("Справочник производителей"), frameDictionary);
    m_producer->setIcon(QIcon("data/picture/sidebar/producers.ico"));
    connect(m_producer, SIGNAL(clicked()), this, SLOT(slotProdusersDictionary()));

    QCommandLinkButton *m_contact = new QCommandLinkButton(tr("Контакты"), tr("Справочник контактов"), frameDictionary);
    m_contact->setIcon(QIcon("data/picture/sidebar/contacts.ico"));
    connect(m_contact, SIGNAL(clicked()), this, SLOT(slotContactsDictionary()));

    QCommandLinkButton *m_position = new QCommandLinkButton (tr("Должности"), tr("Справочник должностей"), frameDictionary);
    m_position->setIcon(QIcon("data/picture/sidebar/positions.ico"));
    connect(m_position, SIGNAL(clicked()), this, SLOT(slotPositionsDictionary()));

    QCommandLinkButton *m_tasktype = new QCommandLinkButton (tr("Типы задач"), tr("Справочник типов задач"), frameDictionary);
    m_tasktype->setIcon(QIcon("data/picture/sidebar/tasktype.ico"));
    connect(m_tasktype, SIGNAL(clicked()), this, SLOT(slotTaskTypesDictionary()));

    QCommandLinkButton *m_contractor = new QCommandLinkButton(tr("Контрагенты"), tr("Справочник контрагентов"), frameDictionary);
    m_contractor->setIcon(QIcon("data/picture/sidebar/contractortype.ico"));
    connect(m_contractor, SIGNAL(clicked()), this, SLOT(slotContractorTypesDictionary()));

    QCommandLinkButton *m_status = new QCommandLinkButton (tr("Статусы"), tr("Справочник статусов"), frameDictionary);
    m_status->setIcon(QIcon("data/picture/sidebar/status.ico"));
    connect(m_status, SIGNAL(clicked()), this, SLOT(slotStatusDictionary()));

    QCommandLinkButton *m_priority = new QCommandLinkButton (tr("Приоритеты"), tr("Справочник приоритетов"), frameDictionary);
    m_priority->setIcon(QIcon("data/picture/sidebar/priority.ico"));
    connect(m_priority, SIGNAL(clicked()), this, SLOT(slotPrioritiesDictionary()));

    QCommandLinkButton *m_cityandcountry = new QCommandLinkButton (tr("Страны и города"), tr("Справочник стран и городов"), frameDictionary);
    m_cityandcountry->setIcon(QIcon("data/picture/sidebar/countryandcity.ico"));
    connect(m_cityandcountry, SIGNAL(clicked()), this, SLOT(slotCountryAndCityDictionary()));

    QVBoxLayout *vboxDictionary = new QVBoxLayout(frameDictionary);
    vboxDictionary->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    vboxDictionary->addWidget(m_client);
    vboxDictionary->addWidget(m_supplier);
    vboxDictionary->addWidget(m_producer);
    vboxDictionary->addWidget(m_contact);
    vboxDictionary->addWidget(m_position);
    vboxDictionary->addWidget(m_tasktype);
    vboxDictionary->addWidget(m_contractor);
    vboxDictionary->addWidget(m_status);
    vboxDictionary->addWidget(m_priority);
    vboxDictionary->addWidget(m_cityandcountry);

    dockDictionary->setLayout(vboxDictionary);
    dockDictionary->setWidget(frameDictionary);

    QDockWidget *dockFolder = new QDockWidget(tr("Папки"));
    dockFolder->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QFrame *frameFolder = new QFrame(this);

    QCommandLinkButton *m_relation = new QCommandLinkButton(tr("Взаимоотношения"), frameFolder);
    m_relation->setIcon(QIcon("data/picture/sidebar/relations.ico"));
    connect(m_relation, SIGNAL(clicked()), this, SLOT(slotRelationsFolder()));

    QCommandLinkButton *m_calendar = new QCommandLinkButton (tr("Календарь"), frameFolder);
    m_calendar->setIcon(QIcon("data/picture/sidebar/calendar.ico"));
    connect(m_calendar, SIGNAL(clicked()), this, SLOT(slotCalendarFolder()));

    QCommandLinkButton *m_reminder = new QCommandLinkButton (tr("Напоминания"), frameFolder);
    m_reminder->setIcon(QIcon("data/picture/sidebar/reminder.ico"));
    connect(m_reminder, SIGNAL(clicked()), this, SLOT(slotReminderFolder()));

    QVBoxLayout *vboxFolder = new QVBoxLayout(frameFolder);
    vboxFolder->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    vboxFolder->addWidget(m_relation);
    vboxFolder->addWidget(m_calendar);
    vboxFolder->addWidget(m_reminder);

    dockFolder->setLayout(vboxFolder);
    dockFolder->setWidget(frameFolder);

    QDockWidget *dockAdditionally = new QDockWidget(tr("Дополнительно"));
    dockAdditionally->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QFrame *frameAdditionally = new QFrame(this);

    QCommandLinkButton *m_accounting = new QCommandLinkButton(tr("Учетные операции"), tr("Справочник УО"), frameAdditionally);
    m_accounting->setIcon(QIcon("data/picture/sidebar/accounting.png"));
    connect(m_accounting, SIGNAL(clicked()), this, SLOT(slotAccountingAdditionally()));

    QVBoxLayout *vboxAdditionally = new QVBoxLayout(frameAdditionally);
    vboxAdditionally->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    vboxAdditionally->addWidget(m_accounting);

    dockAdditionally->setLayout(vboxAdditionally);
    dockAdditionally->setWidget(frameAdditionally);

    addDockWidget(Qt::LeftDockWidgetArea, dockDictionary);
    addDockWidget(Qt::LeftDockWidgetArea, dockFolder);
    addDockWidget(Qt::LeftDockWidgetArea, dockAdditionally);

    tabifyDockWidget(dockDictionary, dockFolder);
    tabifyDockWidget(dockFolder, dockAdditionally);
}

void LogisticMainWindow::slotNewDocument()
{
    execAction("slotCreateEditDialog", (int)0);
}

void LogisticMainWindow::slotOpenDocument()
{
    execAction("slotCreateEditDialog", (int)1);
}

void LogisticMainWindow::slotSaveDocument()
{
}

void LogisticMainWindow::slotPrintDocument()
{
}

void LogisticMainWindow::slotCutDocument()
{
    execAction("slotCutRecords", QVariant());
}

void LogisticMainWindow::slotCopyDocument()
{
    execAction("slotCopyRecords", QVariant());
}

void LogisticMainWindow::slotPasteDocument()
{
    execAction("slotPasteRecords", QVariant());
}

void LogisticMainWindow::slotDeleteDocument()
{
    execAction("slotDeleteRecords", QVariant());
}

//void LogisticMainWindow::slotSearchDocument()
//{
//}

//void LogisticMainWindow::slotSearchNextDocument()
//{
//}

//void LogisticMainWindow::slotSearchPrevDocument()
//{
//}

void LogisticMainWindow::slotRefreshDocument()
{
    execAction("slotRefreshRecords", QVariant());
}

void LogisticMainWindow::slotOwnerDocument()
{
}

void LogisticMainWindow::slotRelationsFolder()
{
}

void LogisticMainWindow::slotCalendarFolder() const
{
}

void LogisticMainWindow::slotReminderFolder()
{
}

void LogisticMainWindow::slotCustomerDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Заказчики");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createCustomer(),
                            "Заказчики",
                            QIcon(QString("data/picture/sidebar/customer.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotSuppliersDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Поставщики");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createSappliers(),
                            "Поставщики",
                            QIcon(QString("data/picture/sidebar/suppliers.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotProdusersDictionary()
{
}

void LogisticMainWindow::slotContactsDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Контакты");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createContacts(),
                            "Контакты",
                            QIcon(QString("data/picture/sidebar/contacts.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotPositionsDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Должности");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createPosition(),
                            "Должности",
                            QIcon(QString("data/picture/sidebar/positions.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotTaskTypesDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Типы задач");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createTaskType(),
                            "Типы задач",
                            QIcon(QString("data/picture/sidebar/tasktype.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotContractorTypesDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Типы контрагентов");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createContractorType(),
                            "Типы контрагентов",
                            QIcon(QString("data/picture/sidebar/contractortype.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotStatusDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Статусы");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createStatus(),
                            "Статусы",
                            QIcon(QString("data/picture/sidebar/status.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotPrioritiesDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Приоритеты");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createPriorities(),
                            "Приоритеты",
                            QIcon(QString("data/picture/sidebar/priority.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotCountryAndCityDictionary()
{
    QMdiSubWindow *msw = this->findMdiWindow("Страны и города");
    if (!msw){
        this->showMdiWindow(LogisticApplication::createCountryAndCity(),
                            "Страны и города",
                            QIcon(QString("data/picture/sidebar/countryandcity.ico")));
    } else {
        msw->setFocus();
    }
}

void LogisticMainWindow::slotActualDocumentsView()
{
    if (activeMdiWindow()){

        if ( LogisticApplication::instance()->positions != nullptr ){
            if (activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->positions->metaObject()->className()) {
                execAction("slotSelectRecords", LogisticApplication::instance()->positions->actualRecords);
            }
        }

        if ( LogisticApplication::instance()->contractortype != nullptr ){
            if (activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->contractortype->metaObject()->className()) {
                execAction("slotSelectRecords", LogisticApplication::instance()->contractortype->actualRecords);
            }
        }


        if ( LogisticApplication::instance()->tasktype != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->tasktype->metaObject()->className() ) {
                execAction("slotSelectRecords", LogisticApplication::instance()->tasktype->actualRecords);
            }
        }

        if ( LogisticApplication::instance()->status != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->status->metaObject()->className() ) {
                execAction("slotSelectRecords", LogisticApplication::instance()->status->actualRecords);
            }
        }

        if ( LogisticApplication::instance()->priorities != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->priorities->metaObject()->className() ) {
                execAction("slotSelectRecords", LogisticApplication::instance()->priorities->actualRecords);
            }
        }

        if ( LogisticApplication::instance()->countryandcity != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->countryandcity->metaObject()->className() ) {
                execAction("slotSelectRecords", LogisticApplication::instance()->countryandcity->actualRecords);
            }
        }

//        if ( LogisticApplication::instance()->accountingoperation != nullptr ) {
//            if ( activeMdiWindow()->accessibleDescription() == LogisticApplication::instance()->accountingoperation->metaObject()->className() ) {
//                execAction("slotSelectRecords", LogisticApplication::instance()->accountingoperation->actualRecords);
//            }
//        }

    }
}

void LogisticMainWindow::slotTreeDocumentsView()
{
}

void LogisticMainWindow::connectionWindowShow()
{
    LogisticApplication::connectionDialog(this)->show();
}

MdiWindow *LogisticMainWindow::activeMdiWindow()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<MdiWindow *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *LogisticMainWindow::findMdiWindow(const QString &title)
{
    foreach (QMdiSubWindow *window, m_mdiArea->subWindowList()) {
        MdiWindow *m_mdiWindow = qobject_cast<MdiWindow *>(window->widget());
        if (m_mdiWindow->windowTitle() == title)
            return window;
    }
    return 0;
}

void LogisticMainWindow::closeEvent(QCloseEvent *)
{
    m_mdiArea->closeAllSubWindows();
    deleteLater();
}

void LogisticMainWindow::slotGeometryChangeRequested(const QRect &geometry)
{
    setGeometry(geometry);
}

void LogisticMainWindow::slotSwitchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void LogisticMainWindow::slotSetActiveSubWindow(QWidget *window)
{
    if (!window) return;
    m_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void LogisticMainWindow::slotAccountingAdditionally()
{
//    QMdiSubWindow *msw = this->findMdiWindow(tr("Учетные операции"));
//    if (!msw){
//        this->showMdiWindow(LogisticApplication::createAccountingOperation(),
//                            tr("Учетные операции"),
//                            QIcon(QString("data/picture/sidebar/accounting.png")));
//    } else {
//        msw->setFocus();
//    }
}

void LogisticMainWindow::showMdiWindow(QWidget *widget, const QString &title, const QIcon &icon)
{
    LogisticApplication::createMdiMindow(title, icon)->show();
    QHBoxLayout *hbl = new QHBoxLayout;

    hbl->layout()->addWidget(widget);
    activeMdiWindow()->setLayout(hbl);
    activeMdiWindow()->setAccessibleDescription(widget->metaObject()->className());
    m_mdiArea->tileSubWindows();
}



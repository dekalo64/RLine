#include "source/crm_core/core_logisticmainwindow.h"
#include "source/crm_core/core_logisticnamespace.h"

CLogisticMainWindow *CLogisticMainWindow::self = nullptr;

CLogisticMainWindow::CLogisticMainWindow(QWidget *parent, Qt::WindowFlags flags):
    QMainWindow(parent, flags)
{
    setUpdatesEnabled(true);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QRect rect = geometry();
          rect.moveCenter(QApplication::desktop()->availableGeometry().center());
    setGeometry(rect);

    showMaximized();

    mdiArea = new QMdiArea();
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setViewMode(QMdiArea::SubWindowView);
    mdiArea->setTabsClosable(true);

    windowMapper = new QSignalMapper(this);

    statusBar()->setVisible(false);
    statusBar()->setSizeGripEnabled(true);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
                 layout->setSpacing(0);
                 layout->setMargin(0);
                 layout->addWidget(mdiArea);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    updateWindowTitle();
    connectionWindowShow(this);

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), SLOT(updateMenus()));
    connect(windowMapper, SIGNAL(mapped(QWidget*)),SLOT(slotSetActiveSubWindow(QWidget*)));
}

CLogisticMainWindow::~CLogisticMainWindow()
{
    if (IS_VALID_PTR(actionBar))     { delete actionBar;    actionBar    = nullptr; }
    if (IS_VALID_PTR(windowMapper))  { delete windowMapper; windowMapper = nullptr; }
    if (IS_VALID_PTR(mdiArea))       { delete mdiArea;      mdiArea      = nullptr; }
    if (IS_VALID_PTR(self))          { delete self;         self         = nullptr; }
}

QSize CLogisticMainWindow::sizeHint() const
{
    QRect desktopRect = QApplication::desktop()->screenGeometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

void CLogisticMainWindow::setupMenu(const bool &visible)
{
    if (!visible) return;

// Document
    QMenu *documentMenu = menuBar()->addMenu(tr("&Документ"));
    documentMenu->addAction(tr("Новый"), this, SLOT(slotNewDocument()), QKeySequence(Qt::Key_F3));
    documentMenu->addAction(tr("Открыть"), this, SLOT(slotOpenDocument()), QKeySequence(Qt::Key_F4));
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
    dictionaryMenu->addAction(tr("Страны и города"), this, SLOT(slotCountryCityDictionary()));

// View
    QMenu *viewMenu = menuBar()->addMenu(tr("&Вид"));

    QAction *m_actual = viewMenu->addAction(tr("Актуальные документы"));
    connect(m_actual, SIGNAL(triggered()), SLOT(slotActualDocumentsView()));

    viewMenu->addAction(tr("Дерево документов"), this, SLOT(slotTreeDocumentsView()), QKeySequence(Qt::Key_F9));

// Window
    QMenu *windowMenu = menuBar()->addMenu(tr("&Окно"));

    QAction *close = windowMenu->addAction(tr("Закрыть"));
    connect(close, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    QAction *closeAll = windowMenu->addAction(tr("Закрыть все"));
    connect(closeAll, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    windowMenu->addSeparator();

    QAction *m_tile = windowMenu->addAction(tr("Плитка"));
    connect(m_tile, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    QAction *m_cascade = windowMenu->addAction(tr("Каскад"));
    connect(m_cascade, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    windowMenu->addSeparator();

    QMenu *modeMenu = windowMenu->addMenu(tr("Режим отображения"));
    QAction *m_modeMdi = modeMenu->addAction(tr("Оконный"));
    connect(m_modeMdi, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
    QAction *m_modeWindow = modeMenu->addAction(tr("Вкладки"));
    connect(m_modeWindow, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    windowMenu->addSeparator();

    QAction *next = windowMenu->addAction(tr("Следующее"));
    connect(next, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    QAction *previous = windowMenu->addAction(tr("Предыдущее"));
    connect(previous, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
}

void CLogisticMainWindow::setupToolBar(const bool &visible)
{
    if (!visible) return;

    setUnifiedTitleAndToolBarOnMac(true);

    actionBar = new QToolBar(tr("Действия"), this);
    actionBar->setAllowedAreas(Qt::TopToolBarArea | Qt::RightToolBarArea | Qt::BottomToolBarArea);
    CLogisticMainWindow::addToolBar(actionBar);

    QAction *m_nd = actionBar->addAction(QIcon(QString("data/picture/toolbar/new.png")), tr("Новый документ"));
    connect(m_nd, SIGNAL(triggered()), this, SLOT(slotNewDocument()));

    QAction *m_ne = actionBar->addAction(QIcon(QString("data/picture/toolbar/edit.png")), tr("Открыть документ"));
    connect(m_ne, SIGNAL(triggered()), this, SLOT(slotOpenDocument()));

    actionBar->addSeparator();

    QAction *m_refresh = actionBar->addAction(QIcon(QString("data/picture/toolbar/refresh.png")), tr("Обновить"));
    connect(m_refresh, SIGNAL(triggered()), this, SLOT(slotRefreshDocument()));

    QAction *m_print = actionBar->addAction(QIcon(QString("data/picture/toolbar/print.png")), tr("Печать"));
    connect(m_print, SIGNAL(triggered()), this, SLOT(slotPrintDocument()));

    actionBar->addSeparator();

    QAction *m_cut = actionBar->addAction(QIcon(QString("data/picture/toolbar/cut.png")), tr("Вырезать"));
    connect(m_cut, SIGNAL(triggered()), SLOT(slotCutDocument()));

    QAction *m_copy = actionBar->addAction(QIcon(QString("data/picture/toolbar/copy.png")), tr("Копировать"));
    connect(m_copy, SIGNAL(triggered()), this, SLOT(slotCopyDocument()));

    QAction *m_paste = actionBar->addAction(QIcon(QString("data/picture/toolbar/paste.png")), tr("Вставить"));
    connect(m_paste, SIGNAL(triggered()), SLOT(slotPasteDocument()));

    QAction *m_delete = actionBar->addAction(QIcon(QString("data/picture/toolbar/delete.png")), tr("Удалить"));
    connect(m_delete, SIGNAL(triggered()), this, SLOT(slotDeleteDocument()));

    actionBar->addSeparator();

    QAction *m_actualOrAll = actionBar->addAction(QIcon(QString("data/picture/toolbar/actual.png")), tr("Актуальные/Все"));
    connect(m_actualOrAll, SIGNAL(triggered()), SLOT(slotActualDocumentsView()));

    actionBar->addAction(QIcon(QString("data/picture/toolbar/treelist.png")), tr("Дерево/Список"));

    actionBar->setIconSize(QSize(__ICON__WIDTH, __ICON__HEIGHT));
}

void CLogisticMainWindow::setupStatusBar(const bool &visible)
{
    if (!visible) return;

    statusBar()->setVisible(visible);

    QDate  *d = new QDate();

    QLabel *labelDate = new QLabel(" Дата: <b>" + d->currentDate().toString("ddd, dd/MM/yyyy г. ") + "</b>\t");
            labelDate->setMinimumSize(labelDate->sizeHint());

    QLabel *labelDatabase = new QLabel(" База данных: <b>" + CLogisticApplication::instance()->dbname() + "</b>\t");

    statusBar()->addWidget(labelDate);
    statusBar()->addWidget(labelDatabase);
}

void CLogisticMainWindow::updateWindowTitle(const QString &title)
{
    if (title.isEmpty()) {
        setWindowTitle(tr("CRM R-Line"));
    } else {
        setWindowTitle(tr("%1").arg(title));
    }
}

void CLogisticMainWindow::execAction(const QString &stored, const QVariant &arg)
{
    QGenericArgument genericArgument;

    if (arg.isValid()){
        genericArgument = QGenericArgument(arg.typeName(), arg.data());
    }

    if (activeMdiWindow()){

        if ( CLogisticApplication::instance()->position != nullptr ){
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->position->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->position,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->tasktype != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->tasktype->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->tasktype,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->contractortype != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->contractortype->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->contractortype,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->status != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->status->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->status,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->priority != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->priority->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->priority,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->countrycity != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->countrycity->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->countrycity,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->customer != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->customer->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->customer,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }

        if ( CLogisticApplication::instance()->supplier != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->supplier->metaObject()->className() ) {
                QMetaObject::invokeMethod(CLogisticApplication::instance()->supplier,
                                          QString("%1").arg(stored).toUtf8().constData(), genericArgument);
            }
        }
    }
}

void CLogisticMainWindow::setupDockWindow(const bool &visible)
{
    if (!visible) return;

    QDockWidget *dockDictionary = new QDockWidget(tr("Справочники"));
                 dockDictionary->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QFrame *frameDictionary = new QFrame(this);

    QCommandLinkButton *client = new QCommandLinkButton(tr("Заказчики"), tr("Справочник клиентов"), frameDictionary);
                        client->setIcon(QIcon("data/picture/sidebar/customer.ico"));
    connect(client, SIGNAL(clicked()), this, SLOT(slotCustomerDictionary()));

    QCommandLinkButton *supplier = new QCommandLinkButton(tr("Поставщики"), tr("Справочник поставщиков"), frameDictionary);
                        supplier->setIcon(QIcon("data/picture/sidebar/suppliers.ico"));
    connect(supplier, SIGNAL(clicked()), this, SLOT(slotSuppliersDictionary()));

    QCommandLinkButton *producer = new QCommandLinkButton(tr("Производители"), tr("Справочник производителей"), frameDictionary);
                        producer->setIcon(QIcon("data/picture/sidebar/producers.ico"));
    connect(producer, SIGNAL(clicked()), this, SLOT(slotProdusersDictionary()));

    QCommandLinkButton *contact = new QCommandLinkButton(tr("Контакты"), tr("Справочник контактов"), frameDictionary);
                        contact->setIcon(QIcon("data/picture/sidebar/contacts.ico"));
    connect(contact, SIGNAL(clicked()), this, SLOT(slotContactsDictionary()));

    QCommandLinkButton *position = new QCommandLinkButton (tr("Должности"), tr("Справочник должностей"), frameDictionary);
                        position->setIcon(QIcon("data/picture/sidebar/positions.ico"));
    connect(position, SIGNAL(clicked()), this, SLOT(slotPositionsDictionary()));

    QCommandLinkButton *tasktype = new QCommandLinkButton (tr("Типы задач"), tr("Справочник типов задач"), frameDictionary);
                        tasktype->setIcon(QIcon("data/picture/sidebar/tasktype.ico"));
    connect(tasktype, SIGNAL(clicked()), this, SLOT(slotTaskTypesDictionary()));

    QCommandLinkButton *contractor = new QCommandLinkButton(tr("Контрагенты"), tr("Справочник контрагентов"), frameDictionary);
                        contractor->setIcon(QIcon("data/picture/sidebar/contractortype.ico"));
    connect(contractor, SIGNAL(clicked()), this, SLOT(slotContractorTypesDictionary()));

    QCommandLinkButton *status = new QCommandLinkButton (tr("Статусы"), tr("Справочник статусов"), frameDictionary);
                        status->setIcon(QIcon("data/picture/sidebar/status.ico"));
    connect(status, SIGNAL(clicked()), this, SLOT(slotStatusDictionary()));

    QCommandLinkButton *priority = new QCommandLinkButton (tr("Приоритеты"), tr("Справочник приоритетов"), frameDictionary);
                        priority->setIcon(QIcon("data/picture/sidebar/priority.ico"));
    connect(priority, SIGNAL(clicked()), this, SLOT(slotPrioritiesDictionary()));

    QCommandLinkButton *countrycity = new QCommandLinkButton (tr("Страны и города"), tr("Справочник стран и городов"), frameDictionary);
                        countrycity->setIcon(QIcon("data/picture/sidebar/countryandcity.ico"));
    connect(countrycity, SIGNAL(clicked()), this, SLOT(slotCountryCityDictionary()));

    QVBoxLayout *vboxDictionary = new QVBoxLayout(frameDictionary);
                 vboxDictionary->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
                 vboxDictionary->addWidget(client);
                 vboxDictionary->addWidget(supplier);
                 vboxDictionary->addWidget(producer);
                 vboxDictionary->addWidget(contact);
                 vboxDictionary->addWidget(position);
                 vboxDictionary->addWidget(tasktype);
                 vboxDictionary->addWidget(contractor);
                 vboxDictionary->addWidget(status);
                 vboxDictionary->addWidget(priority);
                 vboxDictionary->addWidget(countrycity);

    dockDictionary->setLayout(vboxDictionary);
    dockDictionary->setWidget(frameDictionary);

    QDockWidget *dockFolder = new QDockWidget(tr("Папки"));
                 dockFolder->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QFrame *frameFolder = new QFrame(this);

    QCommandLinkButton *relation = new QCommandLinkButton(tr("Взаимоотношения"), frameFolder);
                        relation->setIcon(QIcon("data/picture/sidebar/relations.ico"));
    connect(relation, SIGNAL(clicked()), this, SLOT(slotRelationsFolder()));

    QCommandLinkButton *calendar = new QCommandLinkButton (tr("Календарь"), frameFolder);
                        calendar->setIcon(QIcon("data/picture/sidebar/calendar.ico"));
    connect(calendar, SIGNAL(clicked()), this, SLOT(slotCalendarFolder()));

    QCommandLinkButton *reminder = new QCommandLinkButton (tr("Напоминания"), frameFolder);
                        reminder->setIcon(QIcon("data/picture/sidebar/reminder.ico"));
    connect(reminder, SIGNAL(clicked()), this, SLOT(slotReminderFolder()));

    QVBoxLayout *vboxFolder = new QVBoxLayout(frameFolder);
                 vboxFolder->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
                 vboxFolder->addWidget(relation);
                 vboxFolder->addWidget(calendar);
                 vboxFolder->addWidget(reminder);

    dockFolder->setLayout(vboxFolder);
    dockFolder->setWidget(frameFolder);

    QDockWidget *dockAdditionally = new QDockWidget(tr("Дополнительно"));
                 dockAdditionally->setFeatures(QDockWidget::NoDockWidgetFeatures);

    QFrame *frameAdditionally = new QFrame(this);

    QCommandLinkButton *accounting = new QCommandLinkButton(tr("Учетные операции"), tr("Справочник УО"), frameAdditionally);
                        accounting->setIcon(QIcon("data/picture/sidebar/accounting.png"));
    connect(accounting, SIGNAL(clicked()), this, SLOT(slotAccountingAdditionally()));

    QVBoxLayout *vboxAdditionally = new QVBoxLayout(frameAdditionally);
                 vboxAdditionally->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
                 vboxAdditionally->addWidget(accounting);

    dockAdditionally->setLayout(vboxAdditionally);
    dockAdditionally->setWidget(frameAdditionally);

    addDockWidget(Qt::LeftDockWidgetArea, dockDictionary);
    addDockWidget(Qt::LeftDockWidgetArea, dockFolder);
    addDockWidget(Qt::LeftDockWidgetArea, dockAdditionally);

    tabifyDockWidget(dockDictionary, dockFolder);
    tabifyDockWidget(dockFolder, dockAdditionally);
}

void CLogisticMainWindow::slotNewDocument()
{
    execAction("slotCreateEditDialog", QString("add"));
}

void CLogisticMainWindow::slotOpenDocument()
{
    execAction("slotCreateEditDialog", QString("edit"));
}

void CLogisticMainWindow::slotPrintDocument()
{
}

void CLogisticMainWindow::slotCutDocument()
{
    execAction("slotCutRecords");
}

void CLogisticMainWindow::slotCopyDocument()
{
    execAction("slotCopyRecords");
}

void CLogisticMainWindow::slotPasteDocument()
{
    execAction("slotPasteRecords");
}

void CLogisticMainWindow::slotDeleteDocument()
{
    execAction("slotDeleteRecords");
}

void CLogisticMainWindow::slotRefreshDocument()
{
    execAction("slotRefreshRecords");
}

void CLogisticMainWindow::slotOwnerDocument()
{
}

void CLogisticMainWindow::slotRelationsFolder()
{
}

void CLogisticMainWindow::slotCalendarFolder() const
{
}

void CLogisticMainWindow::slotReminderFolder()
{
}

void CLogisticMainWindow::slotCustomerDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Заказчики");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createCustomer(),
                       "Заказчики",
                       QIcon(QString("data/picture/sidebar/customer.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotSuppliersDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Поставщики");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createSappliers(),
                       "Поставщики",
                       QIcon(QString("data/picture/sidebar/suppliers.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotProdusersDictionary()
{
    QMessageBox::information(0, "title", "Производители");
}

void CLogisticMainWindow::slotContactsDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Контакты");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createContacts(),
                       "Контакты",
                       QIcon(QString("data/picture/sidebar/contacts.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotPositionsDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Должности");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createPosition(),
                       "Должности",
                       QIcon(QString("data/picture/sidebar/positions.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotTaskTypesDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Типы задач");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createTaskType(),
                       "Типы задач",
                       QIcon(QString("data/picture/sidebar/tasktype.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotContractorTypesDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Типы контрагентов");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createContractorType(),
                       "Типы контрагентов",
                       QIcon(QString("data/picture/sidebar/contractortype.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotStatusDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Статусы");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createStatus(),
                       "Статусы",
                       QIcon(QString("data/picture/sidebar/status.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotPrioritiesDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Приоритеты");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createPriorities(),
                       "Приоритеты",
                       QIcon(QString("data/picture/sidebar/priority.ico")));
    } else {
         msw->setFocus();
    }
}

void CLogisticMainWindow::slotCountryCityDictionary()
{
    QMdiSubWindow *msw = findMdiWindow("Страны и города");
    if (!msw){
         showMdiWindow(CLogisticApplication::instance()->createCountryCity(),
                       "Страны и города",
                       QIcon(QString("data/picture/sidebar/countryandcity.ico")));
    } else {
         msw->setFocus();
    }
}


void CLogisticMainWindow::slotActualDocumentsView()
{
    if (activeMdiWindow()){

        if ( CLogisticApplication::instance()->position != nullptr ){
            if (activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->position->metaObject()->className()) {
                execAction("slotActualRecords", CLogisticApplication::instance()->position->actualRecords);
            }
        }

        if ( CLogisticApplication::instance()->contractortype != nullptr ){
            if (activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->contractortype->metaObject()->className()) {
                execAction("slotActualRecords", CLogisticApplication::instance()->contractortype->actualRecords);
            }
        }


        if ( CLogisticApplication::instance()->tasktype != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->tasktype->metaObject()->className() ) {
                execAction("slotActualRecords", CLogisticApplication::instance()->tasktype->actualRecords);
            }
        }

        if ( CLogisticApplication::instance()->status != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->status->metaObject()->className() ) {
                execAction("slotActualRecords", CLogisticApplication::instance()->status->actualRecords);
            }
        }

        if ( CLogisticApplication::instance()->priority != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->priority->metaObject()->className() ) {
                execAction("slotActualRecords", CLogisticApplication::instance()->priority->actualRecords);
            }
        }

        if ( CLogisticApplication::instance()->countrycity != nullptr ) {
            if ( activeMdiWindow()->accessibleDescription() == CLogisticApplication::instance()->countrycity->metaObject()->className() ) {
                execAction("slotActualRecords", CLogisticApplication::instance()->countrycity->actualRecords);
            }
        }
    }
}

void CLogisticMainWindow::slotTreeDocumentsView()
{
}

void CLogisticMainWindow::connectionWindowShow(QWidget *parent)
{
    CLogisticApplication::instance()->connectionDialog(parent)->show();
}

MdiWindow *CLogisticMainWindow::activeMdiWindow()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiWindow *>(activeSubWindow->widget());
    return nullptr;
}

QMdiSubWindow *CLogisticMainWindow::findMdiWindow(const QString &title)
{
    for (auto *window : mdiArea->subWindowList()) {
         MdiWindow *mdiWindow = qobject_cast<MdiWindow *>(window->widget());
         if (mdiWindow->windowTitle() == title)
             return window;
    }
    return nullptr;
}

void CLogisticMainWindow::closeEvent(QCloseEvent *)
{
    mdiArea->closeAllSubWindows();
    deleteLater();
}

void CLogisticMainWindow::slotGeometryChangeRequested(const QRect &geometry)
{
    setGeometry(geometry);
}

void CLogisticMainWindow::slotSwitchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void CLogisticMainWindow::slotSetActiveSubWindow(QWidget *window)
{
    if (!window) return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void CLogisticMainWindow::showMdiWindow(QWidget *widget, const QString &title, const QIcon &icon)
{
    CLogisticApplication::instance()->createMdiMindow(title, icon)->show();

    QHBoxLayout *hbl = new QHBoxLayout;
                 hbl->layout()->addWidget(widget);

    activeMdiWindow()->setLayout(hbl);
    activeMdiWindow()->setAccessibleDescription(widget->metaObject()->className());

    mdiArea->tileSubWindows();
}



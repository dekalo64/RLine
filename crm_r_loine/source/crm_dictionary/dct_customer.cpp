#include "ui_dct_customer.h"
#include "ui_dlg_customer.h"
#include "ui_dlg_discount.h"
#include "ui_dlg_additem.h"
#include "ui_dlg_customer_g.h"

#include "source/crm_dictionary/dct_customer.h"
#include "source/crm_additionally/adl_communicate.h"
#include "source/crm_dialog/dlg_message.h"

CCustomer::CCustomer(QWidget *parent) :
    QWidget(parent)
  , actualRecords(false)
  , ui(new Ui::CCustomer)
  , customerDialog(new CCustomerDialog(this)), customer_gDialog(new CCustomer_gDialog(this))
  , discountDialog(new CDiscountDialog(this))
  , addItem(new CAddItem(this))
  , focusedWidget(nullptr)
{
    ui->setupUi(this);

// model
    modelFaces    = new QStandardItemModel(this);
    modelPartner  = new QStandardItemModel(this);
    modelHuman    = new QStandardItemModel(this);

    modelSelectionFaces   = new QItemSelectionModel(modelFaces);
    modelSelectionPartner = new QItemSelectionModel(modelPartner);
    modelSelectionHuman   = new QItemSelectionModel(modelHuman);

// create #temporary table
    QString query ("SELECT * INTO #GroupCustomerDiscounts FROM ViewGroupCustomerDiscounts"
                   "SELECT * INTO #CustomerSubdiller FROM ViewCustomerSubdiller");
    QSqlQuery temporary(currentDatabase());
    temporary.exec(query);

    QSplitter *hSplitter = new QSplitter(Qt::Horizontal);
    QSplitter *vSplitter = new QSplitter(Qt::Vertical);

    QWidget *w1 = new QWidget(this);
    QWidget *w2 = new QWidget(this);

    treeFaces          = new QTreeView (this);
    treePartner        = new CCustomerTreeView (this);
    textEditPartnerComment = new QTextEdit (this);
    textEditPartnerComment->setMaximumWidth(100);
    textEditPartnerComment->setReadOnly(true);
    treeHuman          = new CCustomerTreeView (this);
    textEditHumanComment   = new QTextEdit (this);
    textEditHumanComment->setMaximumWidth(100);
    textEditHumanComment->setReadOnly(true);

    QHBoxLayout *hboxPartner = new QHBoxLayout(w1);
                 hboxPartner->setMargin(0);
                 hboxPartner->addWidget(treePartner);
                 hboxPartner->addWidget(textEditPartnerComment);

    QHBoxLayout *hboxHuman = new QHBoxLayout(w2);
                 hboxHuman->setMargin(0);
                 hboxHuman->addWidget(treeHuman);
                 hboxHuman->addWidget(textEditHumanComment);

    vSplitter->addWidget(w1);
    vSplitter->addWidget(w2);
    vSplitter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    ui->vLayoutUnionPartnerHuman->addWidget(vSplitter);

    hSplitter->addWidget(treeFaces);
    hSplitter->addWidget(vSplitter);
    hSplitter->setStretchFactor(1, 3);
    hSplitter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    ui->hLayoutUnionViews->addWidget(hSplitter);

    treeFaces->setObjectName("treeViewFaces");
    treeFaces->setModel(modelFaces);
    treeFaces->setSelectionModel(modelSelectionFaces);
    treeFaces->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeFaces->installEventFilter(this);

    treePartner->setObjectName("treeViewPartner");
    treePartner->setRootIsDecorated(false);
    treePartner->setAlternatingRowColors(true);
    treePartner->setModel(modelPartner);
    treePartner->setSelectionModel(modelSelectionPartner);
    treePartner->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treePartner->installEventFilter(this);

    treeHuman->setObjectName("treeViewHuman");
    treeHuman->setRootIsDecorated(false);
    treeHuman->setAlternatingRowColors(true);
    treeHuman->setModel(modelHuman);
    treeHuman->setSelectionModel(modelSelectionHuman);
    treeHuman->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeHuman->installEventFilter(this);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);

    QToolButton *telephone = new QToolButton(this);
    QPixmap pixmapTelephone("data/picture/additionally/telephone.png");

    telephone->setIcon(QIcon(pixmapTelephone));
    telephone->setIconSize(QSize(24, 24));
    telephone->setCursor(Qt::PointingHandCursor);
    telephone->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(telephone);

    QToolButton *meeting = new QToolButton(this);
    QPixmap pixmapMeeting("data/picture/additionally/meeting.png");

    meeting->setIcon(QIcon(pixmapMeeting));
    meeting->setIconSize(QSize(24, 24));
    meeting->setCursor(Qt::PointingHandCursor);
    meeting->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(meeting);

    QToolButton *event = new QToolButton(this);
    QPixmap pixmapEvent("data/picture/additionally/event.png");

    event->setIcon(QIcon(pixmapEvent));
    event->setIconSize(QSize(24, 24));
    event->setCursor(Qt::PointingHandCursor);
    event->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(event);

    QToolButton *task = new QToolButton(this);
    QPixmap pixmapTask("data/picture/additionally/task.png");

    task->setIcon(QIcon(pixmapTask));
    task->setIconSize(QSize(24, 24));
    task->setCursor(Qt::PointingHandCursor);
    task->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(task);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    mc.idCustomer      = -1;
    mc.nameCustomer    = QString("");

    root = new QStandardItem(QString("Заказчики"));
    root->setIcon(QIcon("data/picture/additionally/root.png"));
    modelFaces->insertColumns(0, FACES_MODEL_COLUMN_COUNT);
    modelFaces->setItem(0, 0, root);

    QFont font(treeFaces->font());
          font.setBold (true);
    modelFaces->setData(modelFaces->index(0, 0), font, Qt::FontRole);
    modelFaces->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> storage;
                 storage.append(1);
                 storage.append(2);
                 storage.append(3);
    CDictionaryCore::columnHidden(treeFaces, modelFaces, storage);
                 storage.clear();

    root->setChild(modelFaces->rowCount(root->index()), new QStandardItem("Загрузка..."));

    modelPartner->insertColumns(0, PARTNER_MODEL_COLUMN_COUNT);

    modelPartner->setHeaderData(0, Qt::Horizontal, "Контрагенты");
    modelPartner->setHeaderData(1, Qt::Horizontal, "Клиентский №");
    modelPartner->setHeaderData(2, Qt::Horizontal, "Телефон");
    modelPartner->setHeaderData(3, Qt::Horizontal, "Город");
    modelPartner->setHeaderData(4, Qt::Horizontal, "Web");
    modelPartner->setHeaderData(5, Qt::Horizontal, "Руководитель");

    modelHuman->insertColumns(0, HUMAN_MODEL_COLUMN_COUNT);

    modelHuman->setHeaderData(0, Qt::Horizontal, "ФИО");
    modelHuman->setHeaderData(1, Qt::Horizontal, "Отдел");
    modelHuman->setHeaderData(2, Qt::Horizontal, "Должность");
    modelHuman->setHeaderData(3, Qt::Horizontal, "Телефон");
    modelHuman->setHeaderData(4, Qt::Horizontal, "Приоритет");

//    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindCities(QString)));
    connect(treeFaces, SIGNAL(expanded(QModelIndex)),  SLOT(slotFillGroup(QModelIndex)));
    connect(treeFaces, SIGNAL(collapsed(QModelIndex)), SLOT(slotClearGroup(QModelIndex)));
    connect(treeFaces, SIGNAL(clicked(QModelIndex)),   SLOT(slotFillPartner(QModelIndex)));

//    connect(treeViewCountry, SIGNAL(clicked(QModelIndex)),
//            this, SLOT(slotDataChanged(QModelIndex)));
//    connect(editDialogCountry, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
//    connect(editDialogCity, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
//    connect(addItem->ui->buttonSave, SIGNAL(clicked()), countryDialog, SLOT(show()));

    connect(addItem->ui->buttonSave, SIGNAL(clicked()), SLOT(slotShowEditDialog()));
    connect(addItem->ui->buttonSave, SIGNAL(clicked()), addItem, SLOT(close()));

    actualRecords
             ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
             :
               ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CCustomer::~CCustomer()
{
// drop #temporary table
    QString query ("DROP TABLE #GroupCustomerDiscounts"
                   "DROP TABLE #CustomerSubdiller");
    QSqlQuery temporary(currentDatabase());
    temporary.exec(query);

    if (IS_VALID_PTR(focusedWidget))        { delete focusedWidget;            focusedWidget  = nullptr;   }

    if (IS_VALID_PTR(customerDialog))       { delete customerDialog;           customerDialog = nullptr;   }
    if (IS_VALID_PTR(discountDialog))       { delete discountDialog;           discountDialog = nullptr;   }
    if (IS_VALID_PTR(customer_gDialog))     { delete customer_gDialog;         customer_gDialog = nullptr; }

    if (IS_VALID_PTR(ui))                   { delete ui;                       ui             = nullptr;   }
}

bool CCustomer::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<QTreeView*>(treeFaces)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();
            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeFaces->clearFocus();
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void CCustomer::fillFacesModel(const QModelIndex &index, QSqlQuery &stored)
{
   modelFaces->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    QString   it;
    int  ncols = stored.record().count();
    int  row(modelFaces->rowCount(index));

    if (stored.record().field(stored.record().indexOf("grp_parent")).isValid()){
    while(stored.next()){
        if (stored.value(stored.record().indexOf("grp_parent")).toInt() ==
            modelFaces->sibling(index.row(), 1, index).data().toInt()){
                 for (int i = 0; i != ncols; ++i){
                     it = stored.value(i).toString();
                     QStandardItem *item = new QStandardItem(it);
                     if (i == stored.record().indexOf("grp_name"))
                            item->setIcon(QIcon("data/picture/additionally/catalog.png"));

                     if (i == stored.record().indexOf("grp_name")){
                            modelFaces->itemFromIndex(index)->setChild(row, 0, item);
                     }
                     else if (i == stored.record().indexOf("grp_code")){
                            modelFaces->itemFromIndex(index)->setChild(row, 1, item);
                     }
                     else if (i == stored.record().indexOf("grp_parent")){
                            modelFaces->itemFromIndex(index)->setChild(row, 2, item);
                     }
                     else if (i == stored.record().indexOf("grp_actual")){
                            modelFaces->itemFromIndex(index)->setChild(row, 3, item);
                     }

                     if (item)
                         item->setChild(item->rowCount(), 0, new QStandardItem("Загрузка..."));
                 }
                 ++row;
        }
    }
    } else {
        while(stored.next())  {
            for (int i = 0; i != ncols; ++i){
                it = stored.value(i).toString();
                QStandardItem *item = new QStandardItem(it);

                if (i == stored.record().indexOf("par_name_first"))
                    item->setIcon(QIcon("data/picture/additionally/customer.png"));

                if (i == stored.record().indexOf("par_name_first")){
                    modelFaces->itemFromIndex(index)->setChild(row, 0, item);
                }
                else if (i == stored.record().indexOf("par_code")){
                    modelFaces->itemFromIndex(index)->setChild(row, 1, item);
                }
                else if (i == stored.record().indexOf("par_actual")){
                    modelFaces->itemFromIndex(index)->setChild(row, 2, item);
                }
            }
        row++;
        }
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CCustomer::fillPartnerModel(QSqlQuery &stored)
{
    modelPartner->removeRows(0, modelPartner->rowCount(QModelIndex()), QModelIndex());
    modelPartner->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    int  ncols = stored.record().count();
    int  row(0);

    while(stored.next()){
        for (int i = 0; i != ncols; ++i){
            if (i == stored.record().indexOf("par_name_first")){
                modelPartner->setItem(row, 0, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_client_N")){
                modelPartner->setItem(row, 1, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_phone")){
                modelPartner->setItem(row, 2, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("cit_name_first")){
                modelPartner->setItem(row, 3, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_Web")){
                modelPartner->setItem(row, 4, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_Director")){
                modelPartner->setItem(row, 5, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_comment")){
                textEditPartnerComment->setText(stored.value(i).toString());
            }
        }
        ++row;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CCustomer::columnHidden(QTreeView *view, QStandardItemModel *model, const QVector<int> &vector)
{
    QVector<int>::const_iterator it = vector.begin();
    while(it != vector.end()){
        for (int i = 0; i != model->columnCount(); ++i){
            if (i == *it)
                 view->setColumnHidden(i, true);
        }
        ++it;
    }
}

bool CCustomer::fillFormSelectedRecord()
{
    return true;
}

void CCustomer::slotFillGroup(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelFaces->hasChildren(index)){
        modelFaces->removeRows(0, modelFaces->rowCount(index), index);
    }

    list.append((int)GROUP_TYPE_CUSTOMER);
    list.append((int)actualRecords);
    list.append((int)SKIP); // parameter skip
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllReferenceGroups", storageHashTable(list));

    fillFacesModel(index, stored);

    if (root->index() != index){
        if(list.size() > 0)
           list.clear();

        list.append((int)actualRecords);
        list.append((int)index.sibling(index.row(), 1).data(Qt::DisplayRole).toInt());
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadAllCustomers", storageHashTable(list));

    fillFacesModel(index, stored);
    }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CCustomer::slotFillPartner(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (index.sibling(index.row(), 3).data().toString() == nullptr && root->index() != index){

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)index.sibling(index.row(), 1).data().toInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOneCustomer", storageHashTable(list));

        fillPartnerModel(stored);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
    } else {
        modelPartner->removeRows(0, modelPartner->rowCount(QModelIndex()), QModelIndex());
        textEditPartnerComment->clear();
    }
}

void CCustomer::slotClearGroup(const QModelIndex &index)
{
    if (index == root->index()){
        if (modelFaces->hasChildren(index)){
            modelFaces->removeRows(1, modelFaces->rowCount(index), index);
        }
    root->setChild(modelFaces->rowCount(root->index()), new QStandardItem("Загрузка..."));
    } else
    if (modelFaces->hasChildren(index)){
            modelFaces->removeRows(1, modelFaces->rowCount(index), index);
    }
    modelFaces->itemFromIndex(index)->setChild(modelFaces->rowCount(index), new QStandardItem("Загрузка..."));
}

void CCustomer::slotCutRecords()
{
    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelSelectionFaces->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (!modelSelectionFaces->selection().isEmpty() &&
                    modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() != nullptr){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено перемещение группы заказчиков"));
                    return;
                }
            }

            if ((focusedWidget->objectName() == treeFaces->objectName())) {
                mc.idCustomer   =
                        modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toInt();
                mc.nameCustomer =
                        modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 0).data().toString();
            } else {
                mc.idCustomer    =  -1;
                mc.nameCustomer  =  "";
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCustomer::slotCopyRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            unsigned    codeCustomer(0);
            QModelIndex codeGroup;

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (!modelSelectionFaces->selection().isEmpty() &&
                     modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() == nullptr){
                codeCustomer = modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toUInt();
                codeGroup = modelSelectionFaces->currentIndex().parent();
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelSelectionFaces->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (!modelSelectionFaces->selection().isEmpty() &&
                     modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() != nullptr){
                     CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
                     return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelSelectionFaces->isSelected(modelSelectionFaces->currentIndex()) &&
                    modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() == nullptr){
                    CMessage answer(this, "Копирование", "Подтверждаете копирование?");
                    QPushButton *copy   = answer.addButton(QString("Копировать"), QMessageBox::ActionRole);
                    QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                    answer.exec();

                    if (answer.clickedButton() == copy){
                        list.append(codeCustomer);
                        list.append(codeGroup.sibling(codeGroup.row(), 1).data().toUInt());
                        stored = execStored(currentDatabase(), "CopyCustomer", storageHashTable(list));
                        stored.finish();
                   //     slotRefreshRecordsCity();
                    } else if (answer.clickedButton() == cancel){
                        answer.reject();
                    }
                }
            } else
                CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCustomer::slotPasteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelSelectionFaces->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                  if (modelSelectionFaces->isSelected(modelSelectionFaces->currentIndex()) &&  mc.idCustomer > -1) {

                      CMessage answer(this, "Перемещение", "Подтверждаете перемещение?");
                      QPushButton *move   = answer.addButton(QString("Переместить"), QMessageBox::ActionRole);
                      QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                      answer.exec();

                      if (answer.clickedButton() == move){
                          list.append(mc.idCustomer);
                          list.append(modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toUInt());
                          stored = execStored(currentDatabase(), "MoveCustomer", storageHashTable(list));
                          stored.finish();
                    //      slotRefreshRecordsCity();
                          mc.idCustomer    =  -1;
                          mc.nameCustomer  =  "";
                      } else if (answer.clickedButton() == cancel){
                          answer.reject();
                      }
                  } else
                      CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
            } else
                CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
        }  else
             CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCustomer::slotDeleteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelFaces->hasChildren(modelSelectionFaces->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelSelectionFaces->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
               if (!modelSelectionFaces->selection().isEmpty()){
                CMessage answer(this, "Удаление", "Подтверждаете удаление?");
                QPushButton *_delete = answer.addButton(QString("Удалить"), QMessageBox::ActionRole);
                QPushButton *cancel = answer.addButton(QString("Отмена"),  QMessageBox::ActionRole);

                answer.exec();

                if (answer.clickedButton() == _delete){
                    if (modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() != nullptr){
                        list.append(GROUP_TYPE_CUSTOMER);
                        list.append(modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toUInt());
                        stored = execStored(currentDatabase(), "DeleteReferenceGroup", storageHashTable(list));
                        stored.finish();
                    //    slotRefreshRecordsCountry();
                    } else {
                        list.append(modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toUInt());
                        stored = execStored(currentDatabase(), "DeleteCustomer", storageHashTable(list));
                        stored.finish();
                    //    slotRefreshRecordsCity();
                    }
                } else if (answer.clickedButton() == cancel){
                    answer.reject();
                }
              } else
                   CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
            } else
                CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCustomer::slotCreateEditDialog(const QString &action)
{
    if (currentDatabase().isOpen()) {

        QString::compare(action, "add") == 0 ? act = Action::Add : act = Action::Edit;

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if ((modelSelectionFaces->currentIndex() == root->index()) && act == Action::Edit){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (modelSelectionFaces->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, заказчик не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeFaces->objectName()){
                if (fillFormSelectedRecord()){
                    if (act == Action::Add){
                        addItem->show();
                        modelSelectionFaces->currentIndex() == root->index()
                                     ? addItem->ui->radioAddDocument->setEnabled(false)
                                     : addItem->ui->radioAddDocument->setEnabled(true);

                    } else {
                        if (modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() != nullptr ||
                                modelSelectionFaces->currentIndex() == root->index()) {
                            customer_gDialog->show();
                        } else
                            customerDialog->show();
                    }
                }
            } else
               CCommunicate::showing(QString("Не удается выполнить, заказчик или группа не выбраны"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, заказчик или группа не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCustomer::slotShowEditDialog(void)
{
    switch ((int)addItem->ui->radioAddDocument->isChecked()) {
    case 0:
        customer_gDialog->show();
        break;
    case 1:
        customerDialog->show();
        break;
    default:
        break;
    }
}

void CCustomer::slotInsertOrUpdateRecords()
{
//    QList<QVariant> list;
//    QSqlQuery       stored;

//    if (focusedWidget->objectName() == treeViewFaces->objectName()){
//        if (rad == 0){

//            const bool choice = addItem->ui->radioAddDocument->isChecked();

//            if (!choice){

//                const int      parent =  modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toUInt();
//                const QString  text   =  customer_gDialog->ui->lineEditName->text();
//                const bool     actual =  customer_gDialog->ui->checkBoxActual->isChecked();

//                list.append(parent);
//                list.append(text);
//                list.append(GROUP_TYPE_CUSTOMER);
//                list.append(1);
//                list.append(actual);

//                stored = execStored(currentDatabase(), "InsertReferenceGroup", storageHashTable(list));
//                stored.finish();

//            } else{

//                if (modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 3).data().toString() != nullptr){
//                    const int group = modelSelectionFaces->currentIndex().sibling(modelSelectionFaces->currentIndex().row(), 1).data().toUInt();
//                    list.append(group);
//                } else {
//                    const QModelIndex group = modelSelectionFaces->currentIndex().parent();
//                    list.append(group.data().toUInt());
//                }

//                // city
//                // inventory

//                const int     discount     = customerDialog->ui->spinBoxDiscount->value();
//                const QString name         = customerDialog->ui->lineEditName->text();
//                const QString nameEng      = customerDialog->ui->lineEditNameEng->text();
//                const QString address      = customerDialog->ui->lineEditAddress->text();
//                const QString attender     = customerDialog->ui->lineEditAttender->text();
//                const QString comment      = customerDialog->ui->textEditComment->toPlainText();
//                const QString phone        = customerDialog->ui->lineEditPhone->text();
//                const QString fax          = customerDialog->ui->lineEditFax->text();
//                const QString postalIndex  = customerDialog->ui->lineEditIndex->text();
//                const QString account      = customerDialog->ui->lineEditAccount->text();
//                const QString mfo          = customerDialog->ui->lineEditMFO->text();
//                const QString bank         = customerDialog->ui->lineEditInBank->text();
//                const float   payLimit     = customerDialog->ui->spinBoxPrepayment->value();
//                const bool    actual       = customerDialog->ui->checkBoxActual->isChecked();
//                const QString clientNumber = customerDialog->ui->lineEditClientNumber->text();
//                const QString email        = customerDialog->ui->lineEditEmail->text();
//                const QString web          = customerDialog->ui->lineEditWeb->text();
//                const QString director     = customerDialog->ui->lineEditDirector->text();
//                const QString inn          = customerDialog->ui->lineEditINN->text();
//                const QString nds          = customerDialog->ui->lineEditNDS->text();
//                const QString issuance     = customerDialog->ui->dateEditDateIssuance->text();
//                const QString iban         = customerDialog->ui->lineEditIBAN->text();
//                const QString zkpo         = customerDialog->ui->lineEditZKPO->text();

//                list.append(city);         list.append(inventory);
//                list.append(discount);     list.append(name);
//                list.append(nameEng);      list.append(address);
//                list.append(attender);     list.append(comment);
//                list.append(phone);        list.append(fax);
//                list.append(zkpo);         list.append(account);
//                list.append(mfo);          list.append(bank);
//                list.append(payLimit);     list.append(actual);
//                list.append(clientNumber); list.append(email);
//                list.append(web);          list.append(director);
//                list.append(inn);          list.append(nds);
//                list.append(issuance);     list.append(iban);
//                list.append(postalIndex);

//                stored = execStored(currentDatabase(), "InsertCustomer", storageHashTable(list));
//                stored.finish();
//            }










//        } else if (rad == 1){
//            list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
//            list.append(countryDialog->ui->lineEditName->text());
//            list.append(countryDialog->ui->lineEditNameEng->text());
//            list.append(countryDialog->ui->lineEditCityCode->text());
//            list.append((int)countryDialog->ui->checkBoxActual->isChecked());
//            stored = execStored(currentDatabase(), "UpdateCountry", storageHashTable(list));
//            stored.finish();
//        }
//        slotRefreshRecordsCountry();
//        clearEditDialog(countryDialog);
//    }
//    else if (focusedWidget->objectName() == treeViewCity->objectName()){
//        if (rad == 0){
//            list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
//            list.append(cityDialog->ui->lineEditName->text());
//            list.append(cityDialog->ui->lineEditNameEng->text());
//            list.append(cityDialog->ui->lineEditCityCode->text());
//            list.append((int)cityDialog->ui->checkBoxActual->isChecked());
//            stored = execStored(currentDatabase(), "InsertCity", storageHashTable(list));
//            stored.finish();
//        } else if (rad == 1){
//            list.append(modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt());
//            list.append(cityDialog->ui->lineEditName->text());
//            list.append(cityDialog->ui->lineEditNameEng->text());
//            list.append(cityDialog->ui->lineEditCityCode->text());
//            list.append((int)cityDialog->ui->checkBoxActual->isChecked());
//            stored = execStored(currentDatabase(), "UpdateCity", storageHashTable(list));
//            stored.finish();
//        }
//        slotRefreshRecordsCity();
//        clearEditDialog(cityDialog);
//    }
}

CCustomerTreeView::CCustomerTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void CCustomerTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void CCustomerTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void CCustomerTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    CCustomerTreeView *source = qobject_cast<CCustomerTreeView*>(event->source()); // target QLineEdit

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCustomerTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    CCustomerTreeView *source = qobject_cast<CCustomerTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCustomerTreeView::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCustomerTreeView::draging()
{
    QModelIndex *item = new QModelIndex(currentIndex());

    if (item->isValid()){
         QMimeData *mimeData = new QMimeData;
                    mimeData->setText(item->data(Qt::DisplayRole).toString());

         QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(QPixmap("data/picture/additionally/dragdrop.png"));

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
         delete item;
    }
}

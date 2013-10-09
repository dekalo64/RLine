#include "ui_adl_accountingoperation.h"
#include "ui_dlg_additem.h"

#include "source/crm_additionally/adl_accountingoperation.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CAccountingOperation::CAccountingOperation(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::CAccountingOperation)
  , referenceGroupDialog(new RefGrpDialog(this)), inventoriesDialog(new InventoriesDialog(this))
  , addItem(new CAddItem(this))
{
    ui->setupUi(this);

// model
    modelReferenceGroup = new QStandardItemModel(this);
    modelInventories    = new QStandardItemModel(this);

    modelSelectionReferenceGroup = new QItemSelectionModel(modelReferenceGroup);
    modelSelectionInventories    = new QItemSelectionModel(modelInventories);

// create #temporary table
    QString query ("SELECT * INTO #InventoryEntry FROM ViewInventoryEntry");
    QSqlQuery temporary(currentDatabase());
              temporary.exec(query);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeRefGroup     = new CRefGroupTreeView   (this);
    treeInventories  = new CInventoriesTreeView(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
                 hbox->addWidget(treeRefGroup);
                 hbox->addWidget(treeInventories);

    splitter->setLayout(hbox);
    splitter->setCollapsible(0, false);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    ui->hLayoutTreeView->addWidget(splitter);

    treeRefGroup->setObjectName("treeViewReferenceGroup");
    treeRefGroup->setModel(modelReferenceGroup);
    treeRefGroup->setMinimumWidth(230);
    treeRefGroup->setSelectionModel(modelSelectionReferenceGroup);
    treeRefGroup->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeRefGroup->installEventFilter(this);

    treeInventories->setObjectName("treeViewInventories");
    treeInventories->setRootIsDecorated(false);
    treeInventories->setAlternatingRowColors(true);
    treeInventories->setModel(modelInventories);
    treeInventories->setSelectionModel(modelSelectionInventories);
    treeInventories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeInventories->installEventFilter(this);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_ ]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    actualRecords = false;
    focusedWidget = nullptr;

    mi.codeRefGroup      = -1;
    mi.nameRefGroup      = QString("");
    mi.codeInventories   = -1;
    mi.nameInventories   = QString("");

    root = new QStandardItem(QString("Учетные операции"));
    root->setIcon(QIcon("data/picture/additionally/root.png"));
    modelReferenceGroup->insertColumns(0, REFGROUP_MODEL_COLUMN_COUNT);
    modelReferenceGroup->setItem(0, 0, root);

    QFont font(treeRefGroup->font());
          font.setBold (true);
    modelReferenceGroup->setData(modelReferenceGroup->index(0, 0), font, Qt::FontRole);
    modelReferenceGroup->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> storage;
                 storage.append(1);
                 storage.append(2);
                 storage.append(3);
    CDictionaryCore::columnHidden(treeRefGroup, modelReferenceGroup, storage);
                 storage.clear();

    root->setChild(modelReferenceGroup->rowCount(root->index()), new QStandardItem("Загрузка..."));

    modelInventories->insertColumns(0, INVENTORIES_MODEL_COLUMN_COUNT);
    modelInventories->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование учетной операции"));

                 storage.append(0);
                 storage.append(2);
    CDictionaryCore::columnHidden(treeInventories, modelInventories, storage);
                 storage.clear();

    connect(treeRefGroup, SIGNAL(expanded(QModelIndex)),  SLOT(slotFillGroup(QModelIndex)));
    connect(treeRefGroup, SIGNAL(collapsed(QModelIndex)), SLOT(slotClearGroup(QModelIndex)));
    connect(treeRefGroup, SIGNAL(clicked(QModelIndex)),   SLOT(slotFillInventories(QModelIndex)));

    connect(referenceGroupDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(inventoriesDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));

    connect(addItem->ui->buttonSave, SIGNAL(clicked()), referenceGroupDialog, SLOT(show()));
    connect(addItem->ui->buttonSave, SIGNAL(clicked()), addItem, SLOT(close()));

    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindInventories(QString)));

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

CAccountingOperation::~CAccountingOperation()
{
// drop #temporary table
    QString query ("DROP TABLE #InventoryEntry");
    QSqlQuery temporary(currentDatabase());
              temporary.exec(query);

    if (IS_VALID_PTR(focusedWidget))        { delete focusedWidget;            focusedWidget  = nullptr; }
    if (IS_VALID_PTR(addItem))              { delete addItem;                  addItem        = nullptr; }

    if (IS_VALID_PTR(inventoriesDialog))    { delete inventoriesDialog;        inventoriesDialog     = nullptr; }
    if (IS_VALID_PTR(referenceGroupDialog)) { delete referenceGroupDialog;     referenceGroupDialog  = nullptr; }

    if (IS_VALID_PTR(modelSelectionInventories)) { delete modelSelectionInventories;     modelSelectionInventories = nullptr; }
    if (IS_VALID_PTR(modelSelectionReferenceGroup))  { delete modelSelectionReferenceGroup;  modelSelectionReferenceGroup  = nullptr; }
    if (IS_VALID_PTR(modelInventories))              { delete modelInventories;              modelInventories              = nullptr; }
    if (IS_VALID_PTR(modelReferenceGroup))           { delete modelReferenceGroup;           modelReferenceGroup           = nullptr; }

    if (IS_VALID_PTR(ui))                     { delete ui;                     ui                     = nullptr; }
}

QMenu *CAccountingOperation::getContextMenu() const
{
    if (focusedWidget->objectName() == treeRefGroup->objectName()){
        return treeRefGroup->menu;
    } else if (focusedWidget->objectName() == treeInventories->objectName()){
        return treeInventories->menu;
    }
    return nullptr;
}

bool CAccountingOperation::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<QTreeView*>(treeRefGroup)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();

            disconnect(getContextMenu()->actions().at(0), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(2), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(3), SIGNAL(triggered()), 0, 0);

            connect(getContextMenu()->actions().at(0), SIGNAL(triggered()), SLOT(slotCreateEditDialog()));
            connect(getContextMenu()->actions().at(2), SIGNAL(triggered()), SLOT(slotDeleteRecords()));
            connect(getContextMenu()->actions().at(3), SIGNAL(triggered()), SLOT(slotRefreshRecordsRefGroup()));

            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeRefGroup->clearFocus();
            return false;
        }
    } else if (object == qobject_cast<CInventoriesTreeView*>(treeInventories)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();

            disconnect(getContextMenu()->actions().at(0), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(2), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(3), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(4), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(5), SIGNAL(triggered()), 0, 0);
            disconnect(getContextMenu()->actions().at(7), SIGNAL(triggered()), 0, 0);

            connect(getContextMenu()->actions().at(0), SIGNAL(triggered()), SLOT(slotCreateEditDialog()));
            connect(getContextMenu()->actions().at(2), SIGNAL(triggered()), SLOT(slotCutRecords()));
            connect(getContextMenu()->actions().at(3), SIGNAL(triggered()), SLOT(slotCopyRecords()));
            connect(getContextMenu()->actions().at(4), SIGNAL(triggered()), SLOT(slotPasteRecords()));
            connect(getContextMenu()->actions().at(5), SIGNAL(triggered()), SLOT(slotDeleteRecords()));
            connect(getContextMenu()->actions().at(7), SIGNAL(triggered()), SLOT(slotRefreshRecordsInventory()));

            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeInventories->clearFocus();
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void CAccountingOperation::fillReferenceGroupModel(QSqlQuery &stored, const QModelIndex &index)
{
    modelReferenceGroup->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    QString   it;
    int  ncols = stored.record().count();
    int  row(modelReferenceGroup->rowCount(index));

    while(stored.next()){
        if (stored.value(stored.record().indexOf("grp_parent")).toInt() ==
            modelReferenceGroup->sibling(index.row(), 1, index).data().toInt()){
                 for (int i = 0; i != ncols; ++i){
                     it = stored.value(i).toString();
                     QStandardItem *item = new QStandardItem(it);
                     if (i == stored.record().indexOf("grp_name"))
                            item->setIcon(QIcon("data/picture/additionally/catalog.png"));

                     if (i == stored.record().indexOf("grp_name")){
                            modelReferenceGroup->itemFromIndex(index)->setChild(row, 0, item);
                     }
                     else if (i == stored.record().indexOf("grp_code")){
                            modelReferenceGroup->itemFromIndex(index)->setChild(row, 1, item);
                     }
                     else if (i == stored.record().indexOf("grp_parent")){
                            modelReferenceGroup->itemFromIndex(index)->setChild(row, 2, item);
                     }

                     if (item)
                         item->setChild(item->rowCount(), 0, new QStandardItem("Загрузка..."));
                 }
                 ++row;
        }
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CAccountingOperation::slotFillGroup(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelReferenceGroup->hasChildren(index)){
        modelReferenceGroup->removeRows(0, modelReferenceGroup->rowCount(index), index);
    }

    list.append(GROUP_TYPE_ACCOUNTING_OPERATION);
    list.append((int)actualRecords);
    list.append((int)SKIP); // parameter skip
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllReferenceGroups ", storageHashTable(list));

    fillReferenceGroupModel(stored, index);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CAccountingOperation::slotFillInventories(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (root->index() != index){

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append((int)index.sibling(index.row(), 1).data().toInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadAllInventories ", storageHashTable(list));

        fillInventoriesModel(stored);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
    } else {
        modelInventories->removeRows(0, modelInventories->rowCount(QModelIndex()), QModelIndex());
    }
}

void CAccountingOperation::slotClearGroup(const QModelIndex &index)
{
    if (index == root->index()){
        if (modelReferenceGroup->hasChildren(index)){
            modelReferenceGroup->removeRows(1, modelReferenceGroup->rowCount(index), index);
        }
    root->setChild(modelReferenceGroup->rowCount(root->index()), new QStandardItem("Загрузка..."));
    } else
    if (modelReferenceGroup->hasChildren(index)){
            modelReferenceGroup->removeRows(1, modelReferenceGroup->rowCount(index), index);
    }
    modelReferenceGroup->itemFromIndex(index)->setChild(modelReferenceGroup->rowCount(index), new QStandardItem("Загрузка..."));
}

void CAccountingOperation::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

void CAccountingOperation::slotFindInventories(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindInventorys", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillInventoriesModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

void CAccountingOperation::fillInventoriesModel(QSqlQuery &stored)
{
    modelInventories->removeRows(0, modelInventories->rowCount(QModelIndex()), QModelIndex());
    modelInventories->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    int  ncols = stored.record().count();
    int  row(0);

    while(stored.next()){
        for (int i = 0; i != ncols; ++i){
            if (i == stored.record().indexOf("inv_code")){
                modelInventories->setItem(row, 0, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("inv_name")){
                modelInventories->setItem(row, 1, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("inv_group")){
                modelInventories->setItem(row, 2, new QStandardItem(stored.value(i).toString()));
            }
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CAccountingOperation::slotCutRecords()
{
    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if (modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                if (modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex()) ||
                        modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено перемещение учетных операций"));
                    return;
                }
            }

            if ((focusedWidget->objectName() == treeInventories->objectName())) {
                mi.codeInventories   =
                        modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toInt();
                mi.nameInventories =
                        modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
            } else {
                mi.codeInventories  =  -1;
                mi.nameInventories  =  "";
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CAccountingOperation::slotCopyRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            unsigned codeInventory(0);
            unsigned codeGroup(0);

            if (focusedWidget->objectName() == treeInventories->objectName()){
                codeInventory = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data().toUInt();
                codeGroup     = modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data().toUInt();
            }

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if (modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                if (modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex()) ||
                        modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено копирование учетных операций"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if (modelSelectionInventories->isSelected(modelSelectionInventories->currentIndex())){
                    QMessageBox answer;
                                answer.setText(QString(tr("Подтверждаете копирование?")));
                                answer.setWindowTitle(QString(tr("Копирование")));
                                answer.setIcon(QMessageBox::Question);

                    QPushButton *copy   = answer.addButton(QString(tr("Копировать")), QMessageBox::ActionRole);
                    QPushButton *cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

                    answer.exec();

                    if (answer.clickedButton() == copy){
                        list.append(codeInventory);
                        list.append(codeGroup);
                        stored = execStored(currentDatabase(), "CopyInventory", storageHashTable(list));
                        stored.finish();

                        slotRefreshRecordsInventory();

                    } else if (answer.clickedButton() == cancel){
                        answer.reject();
                    }
                } else
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
            } else
                CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CAccountingOperation::slotRefreshRecordsRefGroup()
{
    slotFillGroup(root->index());
}

void CAccountingOperation::slotRefreshRecordsInventory()
{
    slotFillInventories(modelSelectionReferenceGroup->currentIndex());
}

void CAccountingOperation::slotPasteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        mi.codeRefGroup  =
                modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data().toInt();
        mi.nameRefGroup =
                modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 0).data().toString();

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeRefGroup->objectName()  ||
               (focusedWidget->objectName() == treeInventories->objectName())){
                if (modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()  ||
               (focusedWidget->objectName() == treeInventories->objectName())){
                 if (modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex()) &&  mi.codeInventories > -1) {

                QMessageBox answer;
                            answer.setText(QString(tr("Подтверждаете перемещение?")));
                            answer.setWindowTitle(QString(tr("Перемещение")));
                            answer.setIcon(QMessageBox::Question);

                QPushButton *move   = answer.addButton(QString(tr("Переместить")), QMessageBox::ActionRole);
                QPushButton *cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

                answer.exec();

                if (answer.clickedButton() == move){
                    list.append(mi.codeInventories);
                    list.append(mi.codeRefGroup);
                    stored = execStored(currentDatabase(), "MoveInventory", storageHashTable(list));
                    stored.finish();

                    slotRefreshRecordsInventory();

                    mi.codeInventories  =  -1;
                    mi.nameInventories  =  "";
                } else if (answer.clickedButton() == cancel){
                    answer.reject();
                }
              }
            } else
                CCommunicate::showing(QString("Не удается переместить, учетная операция не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CAccountingOperation::slotDeleteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;
    bool            removable(false);

    if (currentDatabase().isOpen()) {

        unsigned code(0);

        if IS_VALID_PTR(focusedWidget){
            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                code = modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data().toUInt();
            } else if (focusedWidget->objectName() == treeInventories->objectName()){
                code = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data().toUInt();
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if (modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()       &&
                    modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex())){
                removable = true;
            }
            if (focusedWidget->objectName() == treeInventories->objectName()          &&
                    modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex())   &&
                    modelSelectionInventories->isSelected(modelSelectionInventories->currentIndex())){
                removable = true;
            }

            if (removable){
                QMessageBox answer;
                            answer.setText(QString(tr("Подтверждаете удаление?")));
                            answer.setWindowTitle(QString(tr("Удаление")));
                            answer.setIcon(QMessageBox::Question);

                QPushButton *_delete = answer.addButton(QString(tr("Удалить")), QMessageBox::ActionRole);
                QPushButton *cancel  = answer.addButton(QString(tr("Отмена")),  QMessageBox::ActionRole);

                answer.exec();

                if (answer.clickedButton() == _delete){
                    if (focusedWidget->objectName() == treeRefGroup->objectName()){

                        list.append(GROUP_TYPE_ACCOUNTING_OPERATION);
                        list.append(code);

                        stored = execStored(currentDatabase(), "DeleteReferenceGroup", storageHashTable(list));
                        stored.finish();

                        slotRefreshRecordsRefGroup();

                    } else if (focusedWidget->objectName() == treeInventories->objectName()){

                        list.append(code);

                        stored = execStored(currentDatabase(), "DeleteInventory", storageHashTable(list));
                        stored.finish();

                        slotRefreshRecordsInventory();

                    }
                } else if (answer.clickedButton() == cancel){
                    answer.reject();
                }
            } else
                CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CAccountingOperation::slotRefreshRecords()
{
    if (IS_VALID_PTR(focusedWidget)){
        if (focusedWidget->objectName() == treeRefGroup->objectName()){
            slotRefreshRecordsRefGroup();
        } else
        if (focusedWidget->objectName() == treeInventories->objectName()){
            slotRefreshRecordsInventory();
        }
    }
}

void CAccountingOperation::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {

        r == 0 ? rad = RecordActionDatabase::ardInsert
               : rad = RecordActionDatabase::ardUpdate;

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                referenceGroupDialog->setWindowTitle(QString("Папка"));
            } else if (focusedWidget->objectName() == treeInventories->objectName()){
                inventoriesDialog->setWindowTitle(QString("Учетная операция"));
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                if (modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeRefGroup->objectName()){
                if (modelSelectionReferenceGroup->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) && (rad == 0) &&
                    modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) && (rad == 1) &&
                    modelSelectionReferenceGroup->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeInventories->objectName()){
                if(!modelSelectionInventories->isSelected(modelSelectionInventories->currentIndex()) && rad == 1){
                    CCommunicate::showing(QString("Не удается править, учетная операция не выбрана"));
                    return;
                }
            }

            if (fillFormSelectedRecord()){
                if (focusedWidget->objectName() == treeRefGroup->objectName()){
                    if (modelSelectionReferenceGroup->currentIndex() != root->index()){
                        referenceGroupDialog->show();
                    }
                } else  if (focusedWidget->objectName() == treeInventories->objectName()){
                        inventoriesDialog->show();
                }
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, учетная операция не выбрана"));
    }else
       CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CAccountingOperation::slotCreateEditDialog()
{
    slotCreateEditDialog(1);
}

bool CAccountingOperation::fillFormSelectedRecord()
{
    if (focusedWidget->objectName() == treeRefGroup->objectName()){

        QList<QVariant> list;
        QSqlQuery stored;

        if (rad == 0){

            referenceGroupDialog->ui->labelUserD->setText(QString("Нет данных"));
            referenceGroupDialog->ui->labelDateD->setText(QString("Нет данных"));

        } else if (rad == 1){

            const uint code = modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data().toUInt();

            list.append(GROUP_TYPE_ACCOUNTING_OPERATION);
            list.append(code);
            stored.setForwardOnly(true);
            stored = execStored(currentDatabase(), "ReadOneReferenceGroup", storageHashTable(list));

            if (stored.numRowsAffected() > 0) {
                while (stored.next()) {
                    const QString  name   = stored.value(stored.record().indexOf("grp_name")).toString();
                    const unsigned actual = stored.value(stored.record().indexOf("grp_actual")).toUInt();
                    const QString  user   = stored.value(stored.record().indexOf("grp_muser")).toString();
                    const QString  date   = stored.value(stored.record().indexOf("grp_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                    referenceGroupDialog->ui->lineEditRefGroup->setText(name);
                    referenceGroupDialog->ui->checkBoxActual->setChecked(actual);
                    referenceGroupDialog->ui->labelUserD->setText(user);
                    referenceGroupDialog->ui->labelDateD->setText(date);
                }
            } else {
                CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
                return false;
            }
            stored.finish();
        }
    } else if (focusedWidget->objectName() == treeInventories->objectName()){
        if (rad == 0){

            inventoriesDialog->ui->labelUserD->setText(QString("Нет данных"));
            inventoriesDialog->ui->labelDateD->setText(QString("Нет данных"));

        } else if (rad == 1){
            QList<QVariant> listInventory;
            QList<QVariant> listEntry;
            QSqlQuery storedInventory;
            QSqlQuery storedEntry;

            const uint code = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data().toUInt();

            listInventory.append(code);
            storedInventory.setForwardOnly(true);
            storedInventory = execStored(currentDatabase(), "ReadOneInventory", storageHashTable(listInventory));

            ushort rowCount(0);
            while (storedInventory.next()){
                rowCount++;
            }

            if (rowCount > 0) {
                storedInventory = execStored(currentDatabase(), "ReadOneInventory", storageHashTable(listInventory));
                while (storedInventory.next()) {
                    const QString  name   = storedInventory.value(storedInventory.record().indexOf("inv_name")).toString();
                    const unsigned actual = storedInventory.value(storedInventory.record().indexOf("inv_actual")).toUInt();
                    const QString  user   = storedInventory.value(storedInventory.record().indexOf("inv_muser")).toString();
                    const QString  date   = storedInventory.value(storedInventory.record().indexOf("inv_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                    inventoriesDialog->ui->lineEditInventories->setText(name);
                    inventoriesDialog->ui->checkBoxActual->setChecked(actual);
                    inventoriesDialog->ui->labelUserD->setText(user);
                    inventoriesDialog->ui->labelDateD->setText(date);
                }
            } else {
                CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
                return false;
            }

            storedInventory.finish();

            storedEntry.setForwardOnly(true);
            storedEntry = execStored(currentDatabase(), "ReadAllInventoryEntries", storageHashTable(listEntry));

            while (storedEntry.next()) {
                    QListWidgetItem *item = new QListWidgetItem;
                                     item->setIcon(QIcon("data/picture/additionally/accountingtransaction.png"));

                                     float factor = storedEntry.value(storedEntry.record().indexOf("ent_factor")).toFloat();
                                     QString ent_name = storedEntry.value(storedEntry.record().indexOf("ent_name")).toString();
                                     item->setText(QString("%1%2\t%3").arg(factor * 100).arg("%").arg(ent_name));
                    inventoriesDialog->listWidgetAccountingTransaction->addItem(item);
            }

            storedEntry.finish();        
        }
    }
  return true;
}

void CAccountingOperation::slotInsertOrUpdateRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusedWidget->objectName() == treeRefGroup->objectName()){
        if (rad == 0){
            list.append(modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 2).data().toUInt());
            list.append(referenceGroupDialog->ui->lineEditRefGroup->text());
            list.append((int)GROUP_TYPE_ACCOUNTING_OPERATION);
            list.append((int)1); // priority parameter
            list.append((int)referenceGroupDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertReferenceGroup", storageHashTable(list));
            stored.finish();
        } else if (rad == 1){
            list.append(modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data().toUInt());
            list.append(referenceGroupDialog->ui->lineEditRefGroup->text());
            list.append((int)GROUP_TYPE_ACCOUNTING_OPERATION);
            list.append((int)1); // priority parameter
            list.append((int)referenceGroupDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateReferenceGroup", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecords();
        clearEditDialog(referenceGroupDialog);
    }
    else
    if (focusedWidget->objectName() == treeInventories->objectName()){
        if (rad == 0){
            list.append(modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt());
            list.append(inventoriesDialog->ui->lineEditInventories->text());
            list.append((int)inventoriesDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertInventory", storageHashTable(list));
            stored.finish();
        } else if (rad == 1){
            list.append(modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt());
            list.append(inventoriesDialog->ui->lineEditInventories->text());
            list.append((int)inventoriesDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateInventory", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecords();
        clearEditDialog(inventoriesDialog);
    }
}

CInventoriesTreeView::CInventoriesTreeView(QWidget *parent) :
    QTreeView(parent)
  , menu(new QMenu(parent))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Вырезать"), this));
    menu->addAction(new QAction(QObject::tr("Копировать"), this));
    menu->addAction(new QAction(QObject::tr("Вставить"), this));
    menu->addAction(new QAction(QObject::tr("Удалить"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Обновить"), this));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(slotCustomContextMenuRequested(const QPoint &)));
}

CInventoriesTreeView::~CInventoriesTreeView()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CInventoriesTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    for (QAction *action : menu->actions()){
            action->setEnabled(true);
    }
    menu->setStyleSheet("QMenu {"
                                "background-color: #f4f4f4;"
                                "margin: 0px;"
                                "border: 1px solid #515151;"
                        "}"
                        "QMenu::item {"
                                "padding: 2px 25px 2px 20px;"
                                "border: 1px solid transparent;"
                        "}"
                        "QMenu::item:selected {"
                                "color: #ffffff;"
                                "background-color: #68a44a;"
                        "}"
                        "QMenu::separator {"
                                "height: 1px;"
                                "background: #aaaaaa;"
                                "margin-left: 10px;"
                                "margin-right: 5px;"
                        "}"
                        "QMenu::indicator {"
                                "width:  10px;"
                                "height: 10px;"
                        "}");
    QModelIndex index = indexAt(pos);
    QVector<int> disable;
    if (index.isValid()){
            disable.append(4);
            disable.append(7);
    } else {
            disable.append(0);
            disable.append(2);
            disable.append(3);
            disable.append(5);
    }
    for (QVector<int>::const_iterator i = disable.begin(); i != disable.end(); ++i){
        if (disable.contains(*i)){
            menu->actions().at(*i)->setEnabled(false);
        }
    }
    menu->exec(viewport()->mapToGlobal(pos));
}

void CInventoriesTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void CInventoriesTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void CInventoriesTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    CInventoriesTreeView *source = qobject_cast<CInventoriesTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CInventoriesTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    CInventoriesTreeView *source = qobject_cast<CInventoriesTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CInventoriesTreeView::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CInventoriesTreeView::draging()
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

CRefGroupTreeView::CRefGroupTreeView(QWidget *parent) :
    QTreeView(parent)
  , menu(new QMenu(parent))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Удалить"), this));
    menu->addAction(new QAction(QObject::tr("Обновить"), this));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(slotCustomContextMenuRequested(const QPoint &)));
}

CRefGroupTreeView::~CRefGroupTreeView()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CRefGroupTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    for (QAction *action : menu->actions()){
            action->setEnabled(true);
    }
    menu->setStyleSheet("QMenu {"
                                "background-color: #f4f4f4;"
                                "margin: 0px;"
                                "border: 1px solid #515151;"
                        "}"
                        "QMenu::item {"
                                "padding: 2px 25px 2px 20px;"
                                "border: 1px solid transparent;"
                        "}"
                        "QMenu::item:selected {"
                                "color: #ffffff;"
                                "background-color: #68a44a;"
                        "}"
                        "QMenu::separator {"
                                "height: 1px;"
                                "background: #aaaaaa;"
                                "margin-left: 10px;"
                                "margin-right: 5px;"
                        "}"
                        "QMenu::indicator {"
                                "width:  10px;"
                                "height: 10px;"
                        "}");
    QModelIndex index = indexAt(pos);
    QVector<int> disable;
    if (index.isValid()){
            disable.append(3);
    } else {
            disable.append(0);
            disable.append(2);
    }
    for (QVector<int>::const_iterator i = disable.begin(); i != disable.end(); ++i){
        if (disable.contains(*i)){
            menu->actions().at(*i)->setEnabled(false);
        }
    }
    menu->exec(viewport()->mapToGlobal(pos));
}

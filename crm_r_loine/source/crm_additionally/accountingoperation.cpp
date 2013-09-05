#include "source/additionally/accountingoperation.h"
#include "ui_accountingoperation.h"
#include "ui_newitemdialog.h"
#include "source/additionally/communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

AccountingOperation::AccountingOperation(QWidget *parent) :
    QWidget(parent)
  , actualRecords(false)
  , ui(new Ui::AccountingOperation)
  , editDialogRefGroup(new RefGrpDialog(this))
  , editDialogInventories(new InventoriesDialog(this))
  , lastFocusedWidget(nullptr)
  , newItem(new NewItemDialog(this))
{
    ui->setupUi(this);

    modelReferenceGroup     = new QStandardItemModel(this);
    modelInventories  = new QStandardItemModel(this);

    modelProxy = new QSortFilterProxyModel(this);
    modelProxy->setSourceModel(modelInventories);
    modelProxy->setFilterKeyColumn(3);
    modelProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    modelSelectionReferenceGroup      = new QItemSelectionModel(modelReferenceGroup);
    modelSelectionInventories   = new QItemSelectionModel(modelProxy);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeViewRefGroup     = new QTreeView   (this);
    treeViewInventories  = new TreeViewInventories(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
                 hbox->addWidget(treeViewRefGroup);
                 hbox->addWidget(treeViewInventories);

    splitter->setLayout(hbox);
    splitter->setCollapsible(0, false);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    ui->hLayoutTreeView->addWidget(splitter);

    treeViewRefGroup->setObjectName("treeViewRefGroup");
    treeViewRefGroup->setModel(modelReferenceGroup);
    treeViewRefGroup->setMinimumWidth(230);
    treeViewRefGroup->setSelectionModel(modelSelectionReferenceGroup);
    treeViewRefGroup->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewRefGroup->installEventFilter(this);

    treeViewInventories->setObjectName("treeViewInventories");
    treeViewInventories->setRootIsDecorated(false);
    treeViewInventories->setAlternatingRowColors(true);
    treeViewInventories->setModel(modelProxy);
    treeViewInventories->setSelectionModel(modelSelectionInventories);
    treeViewInventories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewInventories->installEventFilter(this);

    lineEditSearchToItem = new LineEditAccountingOperation(this);
    lineEditSearchToItem->setObjectName("lineEditSearchToItem");
    lineEditSearchToItem->setPlaceholderText("Введите наименование");
    ui->hLayoutSearchToItem->addWidget(lineEditSearchToItem);

    lineEditSearchToItem->installEventFilter(this);
    lineEditSearchToItem->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_]+")), this));

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    mr.idRefGroup      = -1;
    mr.nameRefGroup    = QString("");
    mr.idInventories   = -1;
    mr.nameInventories = QString("");

    connect(this, SIGNAL(filterWildcard(QString)),
            modelProxy, SLOT(setFilterWildcard(QString)));
    connect(this, SIGNAL(pushSelectRecordData()), this, SLOT(slotPushSelectRecordData()));
    connect(lineEditSearchToItem, SIGNAL(textEdited(QString)),
            modelProxy, SLOT(setFilterWildcard(QString)));
    connect(getClearButton(), SIGNAL(clicked()), SLOT(slotClearSearchToItem()));
    connect(treeViewRefGroup, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex)));
    connect(editDialogRefGroup, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(editDialogInventories, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(newItem->ui->buttonSave, SIGNAL(clicked()), editDialogRefGroup, SLOT(show()));
    connect(newItem->ui->buttonSave, SIGNAL(clicked()), SLOT(slotPushSelectRecordData()));
    connect(newItem->ui->buttonSave, SIGNAL(clicked()), newItem, SLOT(close()));

    slotSelectRecords(actualRecords);
}

AccountingOperation::~AccountingOperation()
{
    if (IS_VALID_PTR(lastFocusedWidget))        { delete lastFocusedWidget;        lastFocusedWidget        = nullptr; }
    if (IS_VALID_PTR(newItem))                  { delete newItem;                  newItem                  = nullptr; }
    if (IS_VALID_PTR(editDialogInventories))    { delete editDialogInventories;    editDialogInventories = nullptr; }
    if (IS_VALID_PTR(editDialogRefGroup))        { delete editDialogRefGroup;        editDialogRefGroup        = nullptr; }
    if (IS_VALID_PTR(modelSelectionInventories))     { delete modelSelectionInventories;     modelSelectionInventories     = nullptr; }
    if (IS_VALID_PTR(modelSelectionReferenceGroup))  { delete modelSelectionReferenceGroup;  modelSelectionReferenceGroup  = nullptr; }
    if (IS_VALID_PTR(modelProxy))             { delete modelProxy;             modelProxy             = nullptr; }
    if (IS_VALID_PTR(modelInventories))              { delete modelInventories;              modelInventories              = nullptr; }
    if (IS_VALID_PTR(modelReferenceGroup))           { delete modelReferenceGroup;           modelReferenceGroup           = nullptr; }

    if (IS_VALID_PTR(ui)) { delete ui;   ui   = nullptr; }
}

QToolButton *AccountingOperation::getClearButton() const
{
    return lineEditSearchToItem->clearButton;
}

bool AccountingOperation::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<LineEditAccountingOperation*>(lineEditSearchToItem)) {
        if (event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyboardCapture(keyEvent)){
                modelProxy->setFilterKeyColumn(1);
                return false;
            }
        }
        if (event->type() == QEvent::KeyRelease){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyboardCapture(keyEvent)){
                modelProxy->setFilterKeyColumn(1);
                return false;
            }
        }
    }
    else
    if (object == qobject_cast<QTreeView*>(treeViewRefGroup)) {
        if (event->type() == QEvent::FocusIn){
            lastFocusedWidget = focusWidget();
            modelProxy->setFilterKeyColumn(3);
            return false;
        } else
        if (event->type() == QEvent::FocusOut){
            treeViewRefGroup->clearFocus();
            return false;
        }
    }
    else
    if (object == qobject_cast<TreeViewInventories*>(treeViewInventories)) {
        if (event->type() == QEvent::FocusIn){
            lastFocusedWidget = focusWidget();
            return false;
        } else
        if (event->type() == QEvent::FocusOut){
            treeViewInventories->clearFocus();
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void AccountingOperation::fillingRefGroupModel(QSqlQuery &stored)
{
    QMultiMap<int, QString>      map_np;
    QMultiMap<QString, QString>  map_nn;
    QList <QStandardItem *>      list;

    modelReferenceGroup->clear();
    modelReferenceGroup->insertColumns(0, REFGROUP_MODEL_COLUMN_COUNT);
    modelReferenceGroup->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    QStandardItem *parentItem = new QStandardItem(QString("Учетные операции").trimmed());
                   parentItem->setIcon(QIcon("data/picture/additionally/root.png"));
    modelReferenceGroup->setItem(0, parentItem);
    QFont m_font(treeViewRefGroup->font());
    m_font.setBold (true);
    modelReferenceGroup->setData(modelReferenceGroup->index(0, 0), m_font, Qt::FontRole);

    map_np = setTreeData (stored, parentItem); // parent : name
    map_nn = namePerIndex(stored, map_np);

    for (QMultiMap<QString, QString>::const_iterator i = map_nn.begin(); i != map_nn.end(); ++i){
        if (list.size() > 0) {
            list.clear();
        }
        list = modelReferenceGroup->findItems(i.value(), Qt::MatchExactly | Qt::MatchRecursive);
        QStandardItem *p;
        if (list.size() > 0){
            foreach (p, list) {
                stored.first();
                do{
                    if (stored.value(stored.record().indexOf("grp_parent")).toInt() != 0 &&
                            (stored.value(stored.record().indexOf("grp_name")).toString() == i.key())){
                        setTreeValue(stored, p, p->rowCount());
                    }
                }
                while(stored.next());
            }
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void AccountingOperation::fillingInventoriesModel(QSqlQuery &stored)
{
    modelInventories->clear();
    modelInventories->insertColumns(0, INVENTORIES_MODEL_COLUMN_COUNT);
    modelInventories->insertRows(   stored.numRowsAffected(), 0);

    unsigned  j(-1);

    unsigned ncols = stored.record().count();
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    do {
        for (unsigned i = 0; i != ncols; ++i){
            QStandardItem *item = new QStandardItem(stored.value(i).toString());
            modelInventories->setItem(j ,i, item);
        }
        ++j;
    }
    while(stored.next());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

QMultiMap<int, QString> AccountingOperation::setTreeData(QSqlQuery &stored, QStandardItem *p)
{
    QMultiMap<int, QString> map;

    stored.first();
    do {
        if (stored.value(stored.record().indexOf("grp_parent")).toInt() == 0){
            setTreeValue(stored, p, p->rowCount());
        } else {
            map.insert(
                        stored.value(stored.record().indexOf("grp_parent")).toInt(),
                        stored.value(stored.record().indexOf("grp_name")).toString());
        }
    }
    while(stored.next());
    return map;
}

QMultiMap<QString, QString> AccountingOperation::namePerIndex(QSqlQuery &stored, QMultiMap<int, QString> &m)
{
    QMultiMap<QString, QString> map;

    stored.first();
    do {
        for (QMultiMap<int, QString>::const_iterator i = m.begin(); i != m.end(); ++i){
            if (stored.value(stored.record().indexOf("grp_code")).toInt() == i.key()){
                map.insert(i.value(), stored.value(stored.record().indexOf("grp_name")).toString()); // name : parent name
            }
        }
    }
    while (stored.next());
    return map;
}

void AccountingOperation::setTreeValue(QSqlQuery &stored, QStandardItem *p, int row)
{
    unsigned ncols = stored.record().count();

    for (unsigned i = 0; i != ncols; ++i){
        switch (i) {
        case 0:
            p->setChild(row, 1, new QStandardItem(stored.value(i).toString()));
            break;
        case 1:
            p->setChild(row, 3, new QStandardItem(stored.value(i).toString()));
            break;
        case 2:
            p->setChild(row, 0, new QStandardItem(QIcon("data/picture/additionally/catalog.png"), stored.value(i).toString()));
            break;
        case 3:
            p->setChild(row, 2, new QStandardItem(stored.value(i).toString()));
            break;
        default:
            break;
        }
    }
}

void AccountingOperation::slotSelectRecordsRefGroup(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    list.append((int)GROUP_TYPE);
    list.append((int)actual);
    list.append((int)SKIP); // параметр skip
    stored.setForwardOnly(false);
    stored = execStored(currentDatabase(), "ReadAllReferenceGroups", storageHashTable(list));

    fillingRefGroupModel(stored);
    modelReferenceGroup->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    for (int i = 0; i != modelReferenceGroup->columnCount(); ++i){
        if (i != 0) {
         //  treeViewRefGroup->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();

    actual ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           :
             ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void AccountingOperation::slotSelectRecordsInventories(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    list.append((int)actual);
    list.append(QVariant::Invalid);
    stored.setForwardOnly(false);
    stored = execStored(currentDatabase(), "ReadAllInventories", storageHashTable(list));

    fillingInventoriesModel(stored);
    modelInventories->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование УО"));

    for (int i = 0; i != modelInventories->columnCount(); ++i){
        if (i != 1) {
            //treeViewInventories->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();

    actual ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           :
             ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void AccountingOperation::slotDataChanged(const QModelIndex &index)
{
    if (index.isValid() && qstrcmp(index.data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
        emit filterWildcard(QString().trimmed());
    } else
    if (index.isValid()){
        emit filterWildcard(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString());
    }
}

void AccountingOperation::slotCutRecords()
{
    if IS_VALID_PTR(lastFocusedWidget){

        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
            if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) &&
                    qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается выполнить,\n УО не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
            if (modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex()) ||
                    modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex())){
                Communicate::showing(QString("Не удается выполнить,\n запрещено перемещение УО"), 3000);
                return;
            }
        }

        if ((lastFocusedWidget->objectName() == treeViewInventories->objectName())) {
            mr.idInventories   =
                    modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toInt();
            mr.nameInventories =
                    modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
        } else {
            mr.idInventories    =  -1;
            mr.nameInventories  =  "";
        }
    } else
    Communicate::showing(QString("Не удается выполнить,\n выберете УО"), 3000);

}

void AccountingOperation::slotCopyRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    unsigned m_codeInventory(0);
    unsigned m_codeGroup(0);

    QString  m_name(QString().trimmed());

    if IS_VALID_PTR(lastFocusedWidget){

        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
              m_codeInventory = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt();
              m_codeGroup = modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt();
              m_name    = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
        }

        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
            if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) &&
                qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается копировать,\n УО не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
            if (modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex()) ||
                modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex())){
                Communicate::showing(QString("Не удается копировать,\n запрещено копирование УО"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
            if (modelSelectionInventories->isSelected(modelSelectionInventories->currentIndex())){
                QMessageBox answer;
                            answer.setText(QString(tr("Подтверждаете копирование <b>%1</b>?").arg(m_name)));
                            answer.setWindowTitle(QString(tr("Копирование")));
                            answer.setIcon(QMessageBox::Question);

                QPushButton *m_copy   = answer.addButton(QString(tr("Копировать")), QMessageBox::ActionRole);
                QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

                answer.exec();

                if (answer.clickedButton() == m_copy){
                    list.append(m_codeInventory);
                    list.append(m_codeGroup);
                    stored = execStored(currentDatabase(), "CopyInventory", storageHashTable(list));
                    stored.finish();
                } else
                if (answer.clickedButton() == m_cancel){
                    answer.reject();
                }
            slotRefreshRecordsInventory();
            emit filterWildcard(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString());
          } else {
                Communicate::showing(QString("Не удается копировать,\n запись не выбрана"), 3000);
            }
        }
    } else {
        Communicate::showing(QString("Не удается копировать,\n запись не выбрана"), 3000);
    }
}

void AccountingOperation::slotRefreshRecordsRefGroup()
{
    slotSelectRecordsRefGroup(!actualRecords);
}

void AccountingOperation::slotRefreshRecordsInventory()
{
    slotSelectRecordsInventories(!actualRecords);
    emit filterWildcard(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString());
}

void AccountingOperation::slotPasteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    mr.idRefGroup  =
                modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toInt();
    mr.nameRefGroup =
                modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 0).data(Qt::DisplayRole).toString();

    if IS_VALID_PTR(lastFocusedWidget){

        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
            if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) &&
                    qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается выполнить,\n УО не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()              &&
            modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex()) &&  mr.idInventories > -1) {

            QMessageBox answer;
                        answer.setText(QString(tr("Подтверждаете перемещение <b>%1</b> в папку <b>%2</b>?").arg(mr.nameInventories).arg(mr.nameRefGroup)));
                        answer.setWindowTitle(QString(tr("Перемещение")));
                        answer.setIcon(QMessageBox::Question);

            QPushButton *m_move   = answer.addButton(QString(tr("Переместить")), QMessageBox::ActionRole);
            QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == m_move){
                list.append(mr.idInventories);
                list.append(mr.idRefGroup);
                stored = execStored(currentDatabase(), "MoveInventory", storageHashTable(list));
                stored.finish();

                mr.idInventories    =  -1;
                mr.nameInventories  =  "";
            } else
            if (answer.clickedButton() == m_cancel){
                answer.reject();
            }
        slotSelectRecordsInventories(actualRecords);
        emit filterWildcard(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString());
      } else {
            Communicate::showing(QString("Не удается переместить,\n запись не выбрана"), 3000);
        }
    }
}

void AccountingOperation::slotDeleteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;
    bool            removable(false);

    unsigned m_code(0);
    QString  m_name(QString().trimmed());

    if IS_VALID_PTR(lastFocusedWidget){
        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
            m_code = modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt();
            m_name = modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString();
        } else
        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
            m_code = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt();
            m_name = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
        }

        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
            if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex())){
                Communicate::showing(QString("Не удается удалить,\n корневая запись"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
            if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) &&
                qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается удалить,\n УО не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()       &&
            modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex())){
                 removable = true;
        }
        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()          &&
            modelSelectionReferenceGroup->isSelected(modelSelectionReferenceGroup->currentIndex())   &&
            modelSelectionInventories->isSelected(modelSelectionInventories->currentIndex()))      {
                 removable = true;
        }
        if (removable){
            QMessageBox answer;
                        answer.setText(QString(tr("Подтверждаете удаление <b>%1</b>?").arg(m_name)));
                        answer.setWindowTitle(QString(tr("Удаление")));
                        answer.setIcon(QMessageBox::Question);

            QPushButton *m_delete = answer.addButton(QString(tr("Удалить")), QMessageBox::ActionRole);
            QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),  QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == m_delete){
                if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){

                    list.append(GROUP_TYPE);
                    list.append(m_code);

                    stored = execStored(currentDatabase(), "DeleteReferenceGroup", storageHashTable(list));
                    stored.finish();
                    slotRefreshRecordsRefGroup();
                    treeViewRefGroup->expandToDepth(0);
                } else
                if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){

                    list.append(m_code);

                    stored = execStored(currentDatabase(), "DeleteInventory", storageHashTable(list));
                    stored.finish();
                    slotRefreshRecordsInventory();
                    emit filterWildcard(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().trimmed());
                }
            } else
                if (answer.clickedButton() == m_cancel){
                    answer.reject();
                }
        } else {
            Communicate::showing(QString("Не удается удалить,\n запись не выбрана"), 3000);
        }
    } else {
        Communicate::showing(QString("Не удается удалить,\n запись не выбрана"), 3000);
    }
}

void AccountingOperation::slotRefreshRecords()
{
    if (IS_VALID_PTR(lastFocusedWidget)){
        if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
            slotRefreshRecordsRefGroup();
        } else
        if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
            slotRefreshRecordsInventory();
        }
    }
}

void AccountingOperation::slotCreateEditDialog(int r)
{
    if (currentDatabase().isOpen()) {
        switch (r) {
        case 0:
             rad = RecordActionDatabase::ardInsert;
            break;
        case 1:
            rad = RecordActionDatabase::ardUpdate;
            break;
        default:
            break;
        }
        if IS_VALID_PTR(lastFocusedWidget){

            if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
                editDialogRefGroup->setWindowTitle(QString("Папка"));
            } else
            if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
                editDialogInventories->setWindowTitle(QString("Учетная операция"));
            }

            if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
                if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) && rad == RecordActionDatabase::ardUpdate &&
                    qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                    Communicate::showing(QString("Не удается править,\n корневая запись"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
                if (modelSelectionReferenceGroup->selection().isEmpty()){
                    Communicate::showing(QString("Не удается выполнить,\nстрана не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
                if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) && (rad == RecordActionDatabase::ardInsert) &&
                    qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                    Communicate::showing(QString("Не удается добавить,\n УО не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
                if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) && (rad == RecordActionDatabase::ardUpdate) &&
                    qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                    Communicate::showing(QString("Не удается править,\n УО не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
                if(!modelSelectionInventories->isSelected(modelSelectionInventories->currentIndex()) && rad == RecordActionDatabase::ardUpdate){
                    Communicate::showing(QString("Не удается править,\n запись не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
                if (rad == RecordActionDatabase::ardInsert) {
                    newItem->show();
                    if (modelReferenceGroup->hasChildren(modelSelectionReferenceGroup->currentIndex()) &&
                        qstrcmp(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Учетные операции").trimmed().toUtf8().constData()) == 0){
                        newItem->ui->radioNewSubCatalog->setEnabled(false);
                    } else
                        newItem->ui->radioNewSubCatalog->setEnabled(true);

                } else {
                    editDialogRefGroup->show();
                }
            } else
                if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
                    editDialogInventories->show();
                }
            emit pushSelectRecordData();
        } else{
            Communicate::showing(QString("Не удается выполнить,\nзапись не выбрана"), 3000);
        }
    }
}

void AccountingOperation::slotPushSelectRecordData()
{
    if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
        if (rad == RecordActionDatabase::ardUpdate){
            QList<QVariant> list;
            QSqlQuery stored;

            uint m_code = modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt();
            list.append(GROUP_TYPE);
            list.append(m_code);
            stored.setForwardOnly(false);
            stored = execStored(currentDatabase(), "ReadOneReferenceGroup", storageHashTable(list));
            stored.first();

            QString m_name = stored.value(stored.record().indexOf("grp_name")).toString();
            editDialogRefGroup->ui->lineEditRefGroup->setText(m_name);

            unsigned m_actual = stored.value(stored.record().indexOf("grp_actual")).toUInt();
            editDialogRefGroup->ui->checkBoxActual->setChecked(m_actual);

            QString m_user = stored.value(stored.record().indexOf("grp_muser")).toString();
            editDialogRefGroup->ui->labelUserD->setText(m_user);

            QString m_date = stored.value(stored.record().indexOf("grp_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            editDialogRefGroup->ui->labelDateD->setText(m_date);

            stored.finish();
        }
    } else
    if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
        if (rad == RecordActionDatabase::ardUpdate){
            QList<QVariant> listInventory;
            QList<QVariant> listEntry;
            QSqlQuery storedInventory;
            QSqlQuery storedEntry;

            uint m_code = modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt();

            listInventory.append(m_code);
            storedInventory.setForwardOnly(false);
            storedInventory = execStored(currentDatabase(), "ReadOneInventory", storageHashTable(listInventory));
            storedInventory.first();

            QString m_name = storedInventory.value(storedInventory.record().indexOf("inv_name")).toString();
            editDialogInventories->ui->lineEditInventories->setText(m_name);

            unsigned m_actual = storedInventory.value(storedInventory.record().indexOf("inv_actual")).toUInt();
            editDialogInventories->ui->checkBoxActual->setChecked(m_actual);

            QString m_user = storedInventory.value(storedInventory.record().indexOf("inv_muser")).toString();
            editDialogInventories->ui->labelUserD->setText(m_user);

            QString m_date = storedInventory.value(storedInventory.record().indexOf("inv_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            editDialogInventories->ui->labelDateD->setText(m_date);

            storedInventory.finish();

            storedEntry.setForwardOnly(false);
            storedEntry = execStored(currentDatabase(), "ReadAllInventoryEntries", storageHashTable(listEntry));
            storedEntry.first();
            do {
                    QListWidgetItem *item = new QListWidgetItem;
                                     item->setIcon(QIcon("data/picture/additionally/accountingtransaction.png"));

                                     float factor = storedEntry.value(storedEntry.record().indexOf("ent_factor")).toFloat();
                                     QString ent_name = storedEntry.value(storedEntry.record().indexOf("ent_name")).toString();
                                     item->setText(QString("%1%2\t%3").arg(factor * 100).arg("%").arg(ent_name));
                    editDialogInventories->listWidgetAccountingTransaction->addItem(item);
            }
            while (storedEntry.next());
            storedEntry.finish();        
        }
    }
}

void AccountingOperation::slotInsertOrUpdateRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (lastFocusedWidget->objectName() == treeViewRefGroup->objectName()){
        if (rad == RecordActionDatabase::ardInsert){
            if (newItem->ui->radioNewCatalog->isChecked()){
                list.append((int)0);
            } else
            list.append(modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 3).data(Qt::DisplayRole).toUInt());
            list.append(editDialogRefGroup->ui->lineEditRefGroup->text());
            list.append((int)GROUP_TYPE);
            list.append((int)1); // priority parameter
            list.append((int)editDialogRefGroup->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertReferenceGroup", storageHashTable(list));
            stored.finish();
        } else
        if (rad == RecordActionDatabase::ardUpdate){
            list.append(modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt());
            list.append(editDialogRefGroup->ui->lineEditRefGroup->text());
            list.append((int)GROUP_TYPE);
            list.append((int)1); // priority parameter
            list.append((int)editDialogRefGroup->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateReferenceGroup", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecords();
        treeViewRefGroup->expandToDepth(0);

        clearEditDialog(editDialogRefGroup);
    }
    else
    if (lastFocusedWidget->objectName() == treeViewInventories->objectName()){
        if (rad == RecordActionDatabase::ardInsert){
            list.append(modelSelectionReferenceGroup->currentIndex().sibling(modelSelectionReferenceGroup->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt());
            list.append(editDialogInventories->ui->lineEditInventories->text());
            list.append((int)editDialogInventories->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertInventory", storageHashTable(list));
            stored.finish();
        } else
        if (rad == RecordActionDatabase::ardUpdate){
            list.append(modelSelectionInventories->currentIndex().sibling(modelSelectionInventories->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt());
            list.append(editDialogInventories->ui->lineEditInventories->text());
            list.append((int)editDialogInventories->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateInventory", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecords();
        emit filterWildcard(modelSelectionReferenceGroup->currentIndex().data(Qt::DisplayRole).toString());

        clearEditDialog(editDialogInventories);
    }
}

void AccountingOperation::slotClearSearchToItem()
{
    lineEditSearchToItem->undo();
    lineEditSearchToItem->backspace();
}

void AccountingOperation::slotSelectRecords(bool actual)
{
    slotSelectRecordsRefGroup(actual);
    slotSelectRecordsInventories(actual);
    actualRecords = !actualRecords;
}

bool AccountingOperation::keyboardCapture(QKeyEvent *event) const
{
    if (((event->nativeVirtualKey() >= 48) && (event->nativeVirtualKey() <= 57))
            ||
       (((event->nativeVirtualKey() >= 65) && (event->nativeVirtualKey() <= 90)))
            ||
        ((event->nativeVirtualKey() >= 97) && (event->nativeVirtualKey() <= 122))) {
        return true;
    }
    return false;
}

TreeViewInventories::TreeViewInventories(QWidget *parent) :
    QTreeView(parent)
{
    setAcceptDrops(true);
}

void TreeViewInventories::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void TreeViewInventories::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void TreeViewInventories::dragEnterEvent(QDragEnterEvent *event)
{
    TreeViewInventories *source = qobject_cast<TreeViewInventories*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeViewInventories::dragMoveEvent(QDragMoveEvent *event)
{
    TreeViewInventories *source = qobject_cast<TreeViewInventories*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeViewInventories::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeViewInventories::draging()
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

LineEditAccountingOperation::LineEditAccountingOperation(QWidget *parent)
   : QLineEdit(parent)
{
   clearButton = new QToolButton(this);
   QPixmap pixmapClear("data/picture/additionally/clear-button.png");

   searchButton = new QToolButton(this);
   QPixmap pixmapSearch("data/picture/additionally/search.png");

   clearButton->setIcon(QIcon(pixmapClear));
   clearButton->setIconSize(pixmapClear.size());
   clearButton->setCursor(Qt::PointingHandCursor);
   clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
   clearButton->hide();

   searchButton->setIcon(QIcon(pixmapSearch));
   searchButton->setIconSize(pixmapSearch.size());
   searchButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");

   connect(this, SIGNAL(textChanged(const QString&)), SLOT(slotUpdateCloseButton(const QString&)));

   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth + 1));
   setStyleSheet(QString("QLineEdit { padding-left: %1px; } ").arg(searchButton->sizeHint().width() + frameWidth + 1));
   QSize msz = minimumSizeHint();
   setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                  qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
   setMinimumSize(qMax(msz.width(), searchButton->sizeHint().height() + frameWidth * 2 + 2),
                  qMax(msz.height(), searchButton->sizeHint().height() + frameWidth * 2 + 2));
}

void LineEditAccountingOperation::resizeEvent(QResizeEvent *)
{
   QSize szClear = clearButton->sizeHint();
   QSize szSearch = searchButton->sizeHint();
   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   clearButton->move(rect().right() - frameWidth - szClear.width(),
                     (rect().bottom() + 1 - szClear.height()) / 2);
   searchButton->move(rect().left() + 4,
                     (rect().bottom() + 1 - szSearch.height()) / 2);
}

void LineEditAccountingOperation::slotUpdateCloseButton(const QString& text)
{
    clearButton->setVisible(!text.isEmpty());
}

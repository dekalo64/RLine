#include "source/crm_dictionary/dct_task.h"
#include "source/crm_additionally/adl_communicate.h"
#include "source/crm_dialog/dlg_message.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CTaskType::CTaskType(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(false)
{
// model
    modelTask          = new QStandardItemModel(this);
    modelSelectionTask = new QItemSelectionModel(modelTask);

    treeCppsst->setRootIsDecorated(false);
    treeCppsst->setAlternatingRowColors(true);
    treeCppsst->setModel(modelTask);
    treeCppsst->setSelectionModel(modelSelectionTask);
    treeCppsst->installEventFilter(this);

    modelTask->insertColumns(0, TASK_MODEL_COLUMN_COUNT);
    modelTask->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> storage;
                 storage.append(0);
                 storage.append(2);
    CDictionaryCore::columnHidden(treeCppsst, modelTask, storage);
                 storage.clear();

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged(QList<QString>)), this, SLOT(slotInsertOrUpdateRecords(QList<QString>)));
    connect(this, SIGNAL(enabledComboBox(bool)), cppsstDialog, SLOT(slotEnabledComboBox(bool)));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindTask(QString)));

    slotFillTask();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CTaskType::~CTaskType()
{
    if (IS_VALID_PTR(modelSelectionTask))  { delete modelSelectionTask;  modelSelectionTask  = nullptr; }
    if (IS_VALID_PTR(modelTask))           { delete modelTask;           modelTask           = nullptr; }
}

bool CTaskType::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<CTreeViewCppsst*>(treeCppsst)) {
        if (event->type() == QEvent::FocusIn){

            for (QAction *action : getContextMenu()->actions()){
                disconnect(action, SIGNAL(triggered()), 0, 0);
            }

            connect(getContextMenu()->actions().at(0), SIGNAL(triggered()), SLOT(slotCreateEditDialog()));
            connect(getContextMenu()->actions().at(2), SIGNAL(triggered()), SLOT(slotCopyRecords()));
            connect(getContextMenu()->actions().at(3), SIGNAL(triggered()), SLOT(slotDeleteRecords()));
            connect(getContextMenu()->actions().at(5), SIGNAL(triggered()), SLOT(slotRefreshRecords()));

            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void CTaskType::slotCreateEditDialog(const QString &action)
{
    if (currentDatabase().isOpen()) {

        QString::compare(action, "add") == 0 ? act = Action::Add : act = Action::Edit;

        cppsstDialog->setWindowTitle(QString("Задача"));

        emit enabledComboBox(false);

        if (treeCppsst == focusWidget()){
            if (act == Action::Add){

                QList<QString> param;
                if (fillListSelectedRecord(param)){
                    cppsstDialog->fillFormSelectedRecord(param, act);
                    cppsstDialog->show();
                }
            } else if (act == Action::Edit){
                if (!modelSelectionTask->selection().isEmpty()){

                    QList<QString> param;
                    if (fillListSelectedRecord(param)){
                        cppsstDialog->fillFormSelectedRecord(param, act);
                        cppsstDialog->show();
                    }
                } else
                    CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
            }
        }else
           CCommunicate::showing(QString("Не удается выполнить, таблица/запись не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CTaskType::slotCreateEditDialog()
{
    slotCreateEditDialog("edit");
}

void CTaskType::fillTaskModel(QSqlQuery &stored)
{
    modelTask->removeRows(0, modelTask->rowCount(QModelIndex()), QModelIndex());
    modelTask->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    QString   it;
    int ncols = stored.record().count();
    int row(0);

    while(stored.next())
    {
        for (int i = 0; i != ncols; ++i){
            it = stored.value(i).toString();
            QStandardItem *item = new QStandardItem(it);
            if (i == stored.record().indexOf("tt_name")){
                   item->setIcon(QIcon("data/picture/sidebar/tasktype.ico"));
            }
            modelTask->setItem(row ,i, item);
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CTaskType::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionTask->selection().isEmpty()){
            CMessage answer(this, "Копирование", "Подтверждаете копирование?");
            QPushButton *copy   = answer.addButton(QString("Копировать"), QMessageBox::ActionRole);
            QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == copy){
                list.append((int)code);
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "CopyTaskType", storageHashTable(list));
                stored.finish();

                slotRefreshRecords(); // refresh

            } else if (answer.clickedButton() == cancel){
                treeCppsst->clearSelection();
                answer.reject();
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}


void CTaskType::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionTask->selection().isEmpty()) {
            CMessage answer(this, "Удаление", "Подтверждаете удаление?");
            QPushButton *_delete = answer.addButton(QString("Удалить"), QMessageBox::ActionRole);
            QPushButton *cancel = answer.addButton(QString("Отмена"),  QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == _delete){
                list.append((int)code);
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "DeleteTaskType", storageHashTable(list));
                stored.finish();

                slotRefreshRecords(); // refresh

            } else if (answer.clickedButton() == cancel){
                treeCppsst->clearSelection();
                answer.reject();
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));

}

void CTaskType::slotRefreshRecords()
{
    slotFillTask();
}

bool CTaskType::fillListSelectedRecord(QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (act == Action::Add) {

        param.append(QString("%1").arg(-1));
        param.append("Нет данных");
        param.append("Нет данных");

    } else if (act == Action::Edit) {

        const int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOneTaskType", storageHashTable(list));

        bool empty(false);
        while (stored.next()){
            empty = true;
        }

        stored = execStored(currentDatabase(), "ReadOneTaskType", storageHashTable(list));

        if (empty) {
            while (stored.next()) {
                const QString name = stored.value(stored.record().indexOf("tt_name")).toString();
                const bool  actual = stored.value(stored.record().indexOf("tt_actual")).toBool();
                const QString user = stored.value(stored.record().indexOf("tt_muser")).toString();
                const QString date = stored.value(stored.record().indexOf("tt_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                param.append(name);
                param.append(QString("%1").arg(-1));
                param.append(QVariant(actual).toString());
                param.append(user);
                param.append(date);
            }
        } else {
            CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
            return false;
        }
    stored.finish();
    }
    return true;
}

void CTaskType::slotInsertOrUpdateRecords(const QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

    list.append(code);
    list.append(param.at(0));
    list.append(QVariant(param.at(2)).toBool());

    if (act == Action::Add) {
        list.removeAt(0);
        stored = execStored(currentDatabase(), "InsertTaskType", storageHashTable(list));
    }
    else if (act == Action::Edit) {
        stored = execStored(currentDatabase(), "UpdateTaskType", storageHashTable(list));
    }
    stored.finish();

    slotRefreshRecords();
    CDictionaryCore::clearEditDialog(cppsstDialog);
}

void CTaskType::slotFillTask()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelTask->rowCount() > 0){
        modelTask->removeRows(0, modelTask->rowCount());
    }

    list.append((int)actualRecords);
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllTaskType", storageHashTable(list));

    fillTaskModel(stored);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CTaskType::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void CTaskType::slotFindTask(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindTaskType", storageHashTable(list));

        bool empty(false);
        while (stored.next()){
            empty = true;
        }

        stored = execStored(currentDatabase(), "FindTaskType", storageHashTable(list));

        if (empty) {
            fillTaskModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

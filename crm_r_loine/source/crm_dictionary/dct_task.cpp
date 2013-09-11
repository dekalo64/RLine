#include "ui_dlg_cppsst.h"

#include "source/crm_dictionary/dct_task.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

TaskType::TaskType(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(true)
{

// model
    modelTask = new QStandardItemModel(this);
    modelSelectionTask  = new QItemSelectionModel(modelTask);

    treeViewCppsst->setRootIsDecorated(false);
    treeViewCppsst->setAlternatingRowColors(true);
    treeViewCppsst->setModel(modelTask);
    treeViewCppsst->setSelectionModel(modelSelectionTask);

    modelTask->insertColumns(0, TASK_MODEL_COLUMN_COUNT);
    modelTask->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    QVector<int> vector;
    columnHidden(treeViewCppsst, modelTask, vector << 0 << 2);
                 vector.clear();

    cppsstDialog->ui->comboBoxIcon->setEnabled (false);
    cppsstDialog->ui->comboBoxIcon->setEditable(false);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindTask(QString)));

    slotFillTask();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

TaskType::~TaskType()
{
    if (IS_VALID_PTR(modelSelectionTask))  { delete modelSelectionTask;  modelSelectionTask  = nullptr; }
    if (IS_VALID_PTR(modelTask))           { delete modelTask;           modelTask           = nullptr; }
}

void TaskType::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {

        r == 0  ? rad = RecordActionDatabase::ardInsert
                : rad = RecordActionDatabase::ardUpdate;

        cppsstDialog->setWindowTitle(QString("Задача"));

        if (treeViewCppsst == focusWidget()){
            if (rad == 0){
                if (fillFormSelectedRecord()){
                    cppsstDialog->show();
                }
            }else if (rad == 1){
                if (!modelSelectionTask->selection().isEmpty()){
                    if (fillFormSelectedRecord()){
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

void TaskType::fillTaskModel(QSqlQuery &stored)
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

void TaskType::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionTask->selection().isEmpty()){
            QMessageBox answer;
                        answer.setText(QString("Подтверждаете копирование?"));
                        answer.setWindowTitle("Копирование");
                        answer.setIcon(QMessageBox::Question);

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
                treeViewCppsst->clearSelection();
                answer.reject();
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}


void TaskType::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionTask->selection().isEmpty()) {
            QMessageBox answer;
                        answer.setText(QString("Подтверждаете удаление?"));
                        answer.setWindowTitle(QString("Удаление"));
                        answer.setIcon(QMessageBox::Question);

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
                treeViewCppsst->clearSelection();
                answer.reject();
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));

}

void TaskType::slotRefreshRecords()
{
    slotFillTask();
}

bool TaskType::fillFormSelectedRecord(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        cppsstDialog->ui->labelUserD->setText(QString("Нет данных"));
        cppsstDialog->ui->labelDateD->setText(QString("Нет данных"));
    } else if (rad == 1) {

        list.append(modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOneTaskType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                cppsstDialog->setWindowTitle(QString(cppsstDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("tt_name")).toString()));
                cppsstDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("tt_name")).toString());
                cppsstDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("tt_actual")).toBool());
                cppsstDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("tt_muser")).toString());
                cppsstDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("tt_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }
        } else {
            CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
            return false;
        }
    stored.finish();
    }
    return true;
}

void TaskType::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "InsertTaskType", storageHashTable(list));
        stored.finish();
    }
    else if (rad == 1) {
        int code = modelSelectionTask->currentIndex().sibling(modelSelectionTask->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "UpdateTaskType", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(cppsstDialog);
}

void TaskType::slotFillTask()
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

void TaskType::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void TaskType::slotFindTask(const QString &text)
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

        if (stored.numRowsAffected() > 0) {
            fillTaskModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

#include "ui_dlg_cppsst.h"

#include "source/crm_dictionary/dct_status.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

Status::Status(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(true)
{

//model
    modelStatus      = new QStandardItemModel(this);
    modelSelectionStatus  = new QItemSelectionModel(modelStatus);

    treeViewCppsst->setRootIsDecorated(false);
    treeViewCppsst->setAlternatingRowColors(true);
    treeViewCppsst->setModel(modelStatus);
    treeViewCppsst->setSelectionModel(modelSelectionStatus);

    modelStatus->insertColumns(0, STATUS_MODEL_COLUMN_COUNT);
    modelStatus->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    QVector<int> vector;
    columnHidden(treeViewCppsst, modelStatus, vector << 0 << 2);
                 vector.clear();

    cppsstDialog->ui->comboBoxIcon->setEnabled (false);
    cppsstDialog->ui->comboBoxIcon->setEditable(false);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindStatus(QString)));

    slotFillStatus();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

Status::~Status()
{
    if (IS_VALID_PTR(modelSelectionStatus))  { delete modelSelectionStatus;  modelSelectionStatus  = nullptr; }
    if (IS_VALID_PTR(modelStatus))           { delete modelStatus;           modelStatus           = nullptr; }
}

void Status::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {

        r == 0  ? rad = RecordActionDatabase::ardInsert
                : rad = RecordActionDatabase::ardUpdate;

        cppsstDialog->setWindowTitle(QString("Статус"));

        if (treeViewCppsst == focusWidget()){
            if (rad == 0){
                if (fillFormSelectedRecord()){
                    cppsstDialog->show();
                }
            }else if (rad == 1){
<<<<<<< HEAD
                if (fillFormSelectedRecord() && !modelSelectionStatus->selection().isEmpty()){
                    cppsstDialog->show();
=======
                if (!modelSelectionStatus->selection().isEmpty()){
                    if (fillFormSelectedRecord()){
                        cppsstDialog->show();
                    }
>>>>>>> 7d34168ed62c3fc6f2c3e44c48dc517eb050ca29
                } else
                    CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
            }
        }else
            CCommunicate::showing(QString("Не удается выполнить, таблица/запись не выбрана"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void Status::fillStatusModel(QSqlQuery &stored)
{
    modelStatus->removeRows(0, modelStatus->rowCount(QModelIndex()), QModelIndex());
    modelStatus->insertRows(stored.numRowsAffected(), 0);

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
            if (i == stored.record().indexOf("st_name")){
                   item->setIcon(QIcon("data/picture/sidebar/status.ico"));
            }
            modelStatus->setItem(row ,i, item);
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void Status::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionStatus->currentIndex().sibling(modelSelectionStatus->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionStatus->selection().isEmpty()){
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
                stored = execStored(currentDatabase(), "CopyStatusType", storageHashTable(list));
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

void Status::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionStatus->currentIndex().sibling(modelSelectionStatus->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionStatus->selection().isEmpty()) {
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
                stored = execStored(currentDatabase(), "DeleteStatusType", storageHashTable(list));
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

void Status::slotRefreshRecords()
{
    slotFillStatus();
}

bool Status::fillFormSelectedRecord(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        cppsstDialog->ui->labelUserD->setText(QString("Нет данных"));
        cppsstDialog->ui->labelDateD->setText(QString("Нет данных"));
    } else if (rad == 1) {

        list.append(modelSelectionStatus->currentIndex().sibling(modelSelectionStatus->currentIndex().row(), 0).data().toUInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOneStatusType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                cppsstDialog->setWindowTitle(QString(cppsstDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("st_name")).toString()));
                cppsstDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("st_name")).toString());
                cppsstDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("st_actual")).toBool());
                cppsstDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("st_muser")).toString());
                cppsstDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("st_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }
        } else {
            CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
            return false;
        }
    stored.finish();
    }
    return true;
}

void Status::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "InsertStatusType", storageHashTable(list));
        stored.finish();
    }
    else if (rad == 1) {
        int code = modelSelectionStatus->currentIndex().sibling(modelSelectionStatus->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "UpdateStatusType", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(cppsstDialog);
}


void Status::slotFillStatus()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelStatus->rowCount() > 0){
        modelStatus->removeRows(0, modelStatus->rowCount());
    }

    list.append((int)actualRecords);
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllStatusType", storageHashTable(list));

    fillStatusModel(stored);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void Status::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

void Status::slotFindStatus(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindStatusType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillStatusModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}


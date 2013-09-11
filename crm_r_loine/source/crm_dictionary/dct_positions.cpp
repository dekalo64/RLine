#include "ui_dlg_cppsst.h"

#include "source/crm_dictionary/dct_positions.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

Positions::Positions(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(true)
{

// model
    modelPositions      = new QStandardItemModel(this);
    modelSelectionPositions  = new QItemSelectionModel(modelPositions);

    treeViewCppsst->setRootIsDecorated(false);
    treeViewCppsst->setAlternatingRowColors(true);
    treeViewCppsst->setModel(modelPositions);
    treeViewCppsst->setSelectionModel(modelSelectionPositions);

    modelPositions->insertColumns(0, POSITIONS_MODEL_COLUMN_COUNT);
    modelPositions->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    QVector<int> vector;
    columnHidden(treeViewCppsst, modelPositions, vector << 0 << 2);
                 vector.clear();

    cppsstDialog->ui->comboBoxIcon->setEnabled (false);
    cppsstDialog->ui->comboBoxIcon->setEditable(false);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindPositions(QString)));

    slotFillPositions();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

Positions::~Positions()
{
    if (IS_VALID_PTR(modelSelectionPositions))  { delete modelSelectionPositions;  modelSelectionPositions  = nullptr; }
    if (IS_VALID_PTR(modelPositions))           { delete modelPositions;           modelPositions           = nullptr; }
}

void Positions::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {

        r == 0  ? rad = RecordActionDatabase::ardInsert
                : rad = RecordActionDatabase::ardUpdate;

        cppsstDialog->setWindowTitle(QString("Должность"));

        if (treeViewCppsst == focusWidget()){
            if (rad == 0){
                if (fillFormSelectedRecord()){
                    cppsstDialog->show();
                }
            }else if (rad == 1){
                if (!modelSelectionPositions->selection().isEmpty()){
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

void Positions::fillPositionsModel(QSqlQuery &stored)
{
    modelPositions->removeRows(0, modelPositions->rowCount(QModelIndex()), QModelIndex());
    modelPositions->insertRows(stored.numRowsAffected(), 0);

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
            if (i == stored.record().indexOf("pos_name")){
                   item->setIcon(QIcon("data/picture/sidebar/positions.ico"));
            }
            modelPositions->setItem(row ,i, item);
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void Positions::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPositions->selection().isEmpty()){
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
                stored = execStored(currentDatabase(), "CopyPositions", storageHashTable(list));
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

void Positions::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPositions->selection().isEmpty()) {
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
                stored = execStored(currentDatabase(), "DeletePositions", storageHashTable(list));
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

void Positions::slotRefreshRecords()
{
    slotFillPositions();
}

bool Positions::fillFormSelectedRecord(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        cppsstDialog->ui->labelUserD->setText(QString("Нет данных"));
        cppsstDialog->ui->labelDateD->setText(QString("Нет данных"));
    } else if (rad == 1) {

        list.append(modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOnePosition", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                cppsstDialog->setWindowTitle(QString(cppsstDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("pos_name")).toString()));
                cppsstDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("pos_name")).toString());
                cppsstDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("pos_actual")).toBool());
                cppsstDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("pos_muser")).toString());
                cppsstDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("pos_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }
        } else {
            CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
            return false;
        }
    stored.finish();
    }
    return true;
}

void Positions::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "InsertPosition", storageHashTable(list));
        stored.finish();
    }
    else if (rad == 1) {
        int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "UpdatePosition", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(cppsstDialog);
}


void Positions::slotFillPositions()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelPositions->rowCount() > 0){
        modelPositions->removeRows(0, modelPositions->rowCount());
    }

    list.append((int)actualRecords);
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllPosition", storageHashTable(list));

    fillPositionsModel(stored);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void Positions::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

void Positions::slotFindPositions(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindPositions", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillPositionsModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

#include "ui_dlg_cppsst.h"

#include "source/crm_dictionary/dct_priorities.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

Priorities::Priorities(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(true)
{
    modelPriorities      = new QStandardItemModel(this);
    modelSelectionPriorities  = new QItemSelectionModel(modelPriorities);

    treeViewCppsst->setRootIsDecorated(false);
    treeViewCppsst->setAlternatingRowColors(true);
    treeViewCppsst->setModel(modelPriorities);
    treeViewCppsst->setSelectionModel(modelSelectionPriorities);

    modelPriorities->insertColumns(0, PRIORITIES_MODEL_COLUMN_COUNT);
    modelPriorities->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    modelPriorities->setHeaderData(3, Qt::Horizontal, QObject::tr("Уровень"));
    QVector<int> vector;
    columnHidden(treeViewCppsst, modelPriorities, vector << 0 << 2);
                 vector.clear();

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    cppsstDialog->ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/red.png"),    "Высокий");
    cppsstDialog->ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/yellow.png"), "Средний");
    cppsstDialog->ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/green.png"),  "Низкий");

    connect(cppsstDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindPriorities(QString)));

    slotFillPriorities();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

Priorities::~Priorities()
{
    if (IS_VALID_PTR(modelSelectionPriorities))  { delete modelSelectionPriorities;  modelSelectionPriorities  = nullptr; }
    if (IS_VALID_PTR(modelPriorities))           { delete modelPriorities;           modelPriorities           = nullptr; }
}

void Priorities::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {

        r == 0  ? rad = RecordActionDatabase::ardInsert
                : rad = RecordActionDatabase::ardUpdate;

        cppsstDialog->setWindowTitle(QString("Приоритет"));

        if (treeViewCppsst == focusWidget()){
            if (rad == 0){
                if (fillFormSelectedRecord()){
                    cppsstDialog->show();
                }
            }else if (rad == 1){
                if (!modelSelectionPriorities->selection().isEmpty()){
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

void Priorities::fillPrioritiesModel(QSqlQuery &stored)
{
    modelPriorities->removeRows(0, modelPriorities->rowCount(QModelIndex()), QModelIndex());
    modelPriorities->insertRows(stored.numRowsAffected(), 0);

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
            if (i == stored.record().indexOf("pt_name")){
                QStandardItem *item = new QStandardItem(it);
                               item->setIcon(QIcon("data/picture/sidebar/priority.ico"));
                               modelPriorities->setItem(row, i, item);
            } else if (i == stored.record().indexOf("pt_level")){
                QStandardItem *item = new QStandardItem();
                unsigned value = stored.value(stored.record().indexOf("pt_level")).toUInt();
                switch (value) {
                case 0:
                    item->setIcon(QIcon("data/picture/additionally/red.png"));
                    break;
                case 1:
                    item->setIcon(QIcon("data/picture/additionally/yellow.png"));
                    break;
                case 2:
                    item->setIcon(QIcon("data/picture/additionally/green.png"));
                    break;
                }
                    modelPriorities->setItem(row, i, item);
            } else
            modelPriorities->setItem(row, i, new QStandardItem(it));
        }
        ++row;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void Priorities::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPriorities->selection().isEmpty()){
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
                stored = execStored(currentDatabase(), "CopyPriorityType", storageHashTable(list));
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

void Priorities::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPriorities->selection().isEmpty()) {
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
                stored = execStored(currentDatabase(), "DeletePriorityType", storageHashTable(list));
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

void Priorities::slotRefreshRecords()
{
    slotFillPriorities();
}

bool Priorities::fillFormSelectedRecord(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        cppsstDialog->ui->comboBoxIcon->setCurrentIndex(-1);
        cppsstDialog->ui->labelUserD->setText(QString("Нет данных"));
        cppsstDialog->ui->labelDateD->setText(QString("Нет данных"));
    } else if (rad == 1) {

        list.append(modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOnePriorityType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                cppsstDialog->setWindowTitle(QString(cppsstDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("pt_name")).toString()));
                cppsstDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("pt_name")).toString());
                cppsstDialog->ui->comboBoxIcon->setCurrentIndex(stored.value(stored.record().indexOf("pt_level")).toUInt());
                cppsstDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("pt_actual")).toBool());
                cppsstDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("pt_muser")).toString());
                cppsstDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("pt_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }
        } else {
            CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
            return false;
        }
    stored.finish();
    }
    return true;
}

void Priorities::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        list.append((int)cppsstDialog->ui->comboBoxIcon->currentIndex());
        stored = execStored(currentDatabase(), "InsertPriorityType", storageHashTable(list));
        stored.finish();
    }
    else if (rad == 1) {
        int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        list.append((int)cppsstDialog->ui->comboBoxIcon->currentIndex());
        stored = execStored(currentDatabase(), "UpdatePriorityType", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(cppsstDialog);
}

void Priorities::slotFillPriorities()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelPriorities->rowCount() > 0){
        modelPriorities->removeRows(0, modelPriorities->rowCount());
    }

    list.append((int)actualRecords);
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllPriorityType", storageHashTable(list));

    fillPrioritiesModel(stored);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void Priorities::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void Priorities::slotFindPriorities(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindPriorityType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillPrioritiesModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

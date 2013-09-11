#include "ui_dlg_cppsst.h"

#include "source/crm_dictionary/dct_contractor.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

ContractorType::ContractorType(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(false)
{

// model
    modelContractor = new QStandardItemModel(this);
    modelSelectionContractor  = new QItemSelectionModel(modelContractor);

    treeViewCppsst->setRootIsDecorated(false);
    treeViewCppsst->setAlternatingRowColors(true);
    treeViewCppsst->setModel(modelContractor);
    treeViewCppsst->setSelectionModel(modelSelectionContractor);

    modelContractor->insertColumns(0, CONTRACTOR_MODEL_COLUMN_COUNT);
    modelContractor->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    QVector<int> vector;
    columnHidden(treeViewCppsst, modelContractor, vector << 0 << 2);
                 vector.clear();

    cppsstDialog->ui->comboBoxIcon->setEnabled (false);
    cppsstDialog->ui->comboBoxIcon->setEditable(false);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindContractor(QString)));

    slotFillContractor();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

ContractorType::~ContractorType()
{
    if (IS_VALID_PTR(modelSelectionContractor)) { delete modelSelectionContractor; modelSelectionContractor  = nullptr; }
    if (IS_VALID_PTR(modelContractor))          { delete modelContractor;          modelContractor           = nullptr; }
}

void ContractorType::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {

        r == 0  ? rad = RecordActionDatabase::ardInsert
                : rad = RecordActionDatabase::ardUpdate;

        cppsstDialog->setWindowTitle(QString("Контрагент"));

        if (treeViewCppsst == focusWidget()){
            if (rad == 0){
                if (fillFormSelectedRecord()){
                    cppsstDialog->show();
                }
            }else if (rad == 1){
                if (!modelSelectionContractor->selection().isEmpty()){
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

void ContractorType::fillContractorModel(QSqlQuery &stored)
{
    modelContractor->removeRows(0, modelContractor->rowCount(QModelIndex()), QModelIndex());
    modelContractor->insertRows(stored.numRowsAffected(), 0);

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
            if (i == stored.record().indexOf("part_name")){
                   item->setIcon(QIcon("data/picture/sidebar/contractortype.ico"));
            }
            modelContractor->setItem(row ,i, item);
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void ContractorType::slotFillContractor(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelContractor->rowCount() > 0){
        modelContractor->removeRows(0, modelContractor->rowCount());
    }

    list.append((int)actualRecords);
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllPartnerType", storageHashTable(list));

    fillContractorModel(stored);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void ContractorType::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void ContractorType::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionContractor->currentIndex().sibling(modelSelectionContractor->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionContractor->selection().isEmpty()){
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
                stored = execStored(currentDatabase(), "CopyPartnerType", storageHashTable(list));
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

void ContractorType::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        int code = modelSelectionContractor->currentIndex().sibling(modelSelectionContractor->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionContractor->selection().isEmpty()) {
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
                stored = execStored(currentDatabase(), "DeletePartnerType", storageHashTable(list));
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

void ContractorType::slotRefreshRecords()
{
    slotFillContractor();
}

bool ContractorType::fillFormSelectedRecord(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        cppsstDialog->ui->labelUserD->setText(QString("Нет данных"));
        cppsstDialog->ui->labelDateD->setText(QString("Нет данных"));
    } else if (rad == 1) {

        list.append(modelSelectionContractor->currentIndex().sibling(modelSelectionContractor->currentIndex().row(), 0).data().toUInt());
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOnePartnerType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                cppsstDialog->setWindowTitle(QString(cppsstDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("part_name")).toString()));
                cppsstDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("part_name")).toString());
                cppsstDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("part_actual")).toBool());
                cppsstDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("part_muser")).toString());
                cppsstDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("part_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }
        } else {
            CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
            return false;
        }
    stored.finish();
    }
    return true;
}

void ContractorType::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (rad == 0) {
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "InsertPartnerType", storageHashTable(list));
        stored.finish();
    }
    else if (rad == 1) {
        int code = modelSelectionContractor->currentIndex().sibling(modelSelectionContractor->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        list.append(cppsstDialog->ui->lineEditName->text());
        list.append((int)cppsstDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "UpdatePartnerType", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(cppsstDialog);
}


void ContractorType::slotFindContractor(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindPartnerType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillContractorModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

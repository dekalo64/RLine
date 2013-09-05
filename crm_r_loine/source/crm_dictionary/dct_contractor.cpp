#include "ui_dlg_cppsst.h"

#include "source/crm_dictionary/dct_contractor.h"
#include "source/crm_additionally/adl_communicate.h"

#define  MODEL_COLUMN_COUNT 5

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

ContractorType::ContractorType(QWidget *parent /* = 0 */):
    DictionaryTemplate(parent)
  , actualRecords(true)
{
    m_model      = new QStandardItemModel(this);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterKeyColumn(1);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel  = new QItemSelectionModel(m_proxyModel);

    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
    treeView->setModel(m_proxyModel);
    treeView->setSelectionModel(m_selectionModel);

    dictionaryDialog->ui->comboBoxIcon->setEnabled (false);
    dictionaryDialog->ui->comboBoxIcon->setEditable(false);

    ui->m_lblCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(this, SIGNAL(pushSelectRecordData()), SLOT(slotPushSelectRecordData()));
    connect(lineEditSearchToItem, SIGNAL(textEdited(QString)),
            m_proxyModel, SLOT(setFilterWildcard(QString)));
    connect(getClearButton(), SIGNAL(clicked()), SLOT(slotClearSearchToItem()));
    connect(dictionaryDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(m_selectionModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex)));
    connect(this, SIGNAL(selectionModelIndex(QModelIndex)), SLOT(slotGetSelectionModel(QModelIndex)));

    slotSelectRecords(actualRecords);
}

ContractorType::~ContractorType()
{
    if (IS_VALID_PTR(m_selectionModel)) { delete m_selectionModel; m_selectionModel  = nullptr; }
    if (IS_VALID_PTR(m_proxyModel))     { delete m_proxyModel;     m_proxyModel      = nullptr; }
    if (IS_VALID_PTR(m_model))          { delete m_model;          m_model           = nullptr; }
}

void ContractorType::fillingModel(QSqlQuery &stored)
{
    m_model->clear();
    m_model->insertColumns(0, MODEL_COLUMN_COUNT);
    m_model->insertRows(0, stored.numRowsAffected());

    unsigned  j(0);
    QString   m_item;

    unsigned ncols = stored.record().count();
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    while(stored.next())
    {
        for (unsigned i = 0; i != ncols; ++i){
            if (stored.value(i).toDateTime().isValid()) {
                m_item = stored.value(i).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            } else {
                m_item = stored.value(i).toString();
            }
            QStandardItem *item = new QStandardItem(m_item.trimmed());
            if (i == 1){
               item->setIcon(QIcon("data/picture/sidebar/contractortype.ico"));
            }
            m_model->setItem(j ,i, item);
        }
        ++j;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void ContractorType::slotSelectRecords(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    list.append((int)actual);
    stored = execStored(currentDatabase(), "ReadAllPartnerType", storageHashTable(list));

    fillingModel(stored);
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));

    for (int i = 0; i != m_model->columnCount(); ++i){
        if (i == 1) {
            continue;
        } else {
            treeView->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();

    actual ? ui->m_lblViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           :
             ui->m_lblViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
    m_selectedItem = false;
    actualRecords = !actualRecords;
}

void ContractorType::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    unsigned i(0);
    unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

    QString  m_name = m_selectionModel->currentIndex().data(Qt::DisplayRole).toString();

    if (m_selectionModel->isSelected(m_selectionModel->currentIndex())){
        QMessageBox answer;
                    answer.setText(QString(tr("Подтверждаете копирование <b>%1</b>?").arg(m_name)));
                    answer.setWindowTitle(tr("Копирование"));
                    answer.setIcon(QMessageBox::Question);

        QPushButton *m_copy   = answer.addButton(QString(tr("Копировать")), QMessageBox::ActionRole);
        QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

        answer.exec();

        if (answer.clickedButton() == m_copy){
            list.append(m_code);
            stored = execStored(currentDatabase(), "CopyPartnerType", storageHashTable(list));
            stored.finish();

            slotRefreshRecords();

        } else if (answer.clickedButton() == m_cancel){
            treeView->clearSelection();
            answer.reject();
        }
    } else {
        CCommunicate::showing(QString("Не удается копировать,\n запись не выбрана"));
    }
    m_selectedItem = false;
}

void ContractorType::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    unsigned i(0);
    unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

    QString  m_name = m_selectionModel->currentIndex().data(Qt::DisplayRole).toString();

    if (m_selectionModel->isSelected(m_selectionModel->currentIndex())){
        QMessageBox answer;
                    answer.setText(QString(tr("Подтверждаете удаление <b>%1</b>?").arg(m_name)));
                    answer.setWindowTitle(QString(tr("Удаление")));
                    answer.setIcon(QMessageBox::Question);

        QPushButton *m_delete = answer.addButton(QString(tr("Удалить")), QMessageBox::ActionRole);
        QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),  QMessageBox::ActionRole);

        answer.exec();

        if (answer.clickedButton() == m_delete){
            list.append(m_code);
            stored = execStored(currentDatabase(), "DeletePartnerType", storageHashTable(list));
            stored.finish();

            slotRefreshRecords();

        } else if (answer.clickedButton() == m_cancel){
            treeView->clearSelection();
            answer.reject();
        }
    } else {
        CCommunicate::showing(QString("Не удается удалить,\n запись не выбрана"));
    }
    m_selectedItem = false;
}

void ContractorType::slotRefreshRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    list.append(!actualRecords);
    stored = execStored(currentDatabase(), "ReadAllPartnerType", storageHashTable(list));

    fillingModel(stored);
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));

    for (int i = 0; i != m_model->columnCount(); ++i){
        if (i == 1) {
            continue;
        } else {
            treeView->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void ContractorType::slotPushSelectRecordData(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    QString         userName;

    if (m_rad == RecordActionDatabase::ardInsert) {
        dictionaryDialog->ui->labelUserD->setText(currentUser());
        dictionaryDialog->ui->labelDateD->setText(QString(tr("Не определено")).trimmed());
    }
    else
    if (m_rad == RecordActionDatabase::ardUpdate) {
        unsigned i(1);
        QString m_item = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toString();
        dictionaryDialog->setWindowTitle(QString(dictionaryDialog->windowTitle() + " запись - [ %1 ]").arg(m_item));
        dictionaryDialog->ui->lineEditItem->setText(QString(m_item).trimmed());

        unsigned c(2);
        unsigned m_check = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), c).data(Qt::DisplayRole).toUInt();
        dictionaryDialog->ui->checkBoxActual->setChecked((bool)m_check);

        unsigned u(3);
        unsigned m_muser = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), u).data(Qt::DisplayRole).toUInt();
        list.append(m_muser);
        stored = execStored(currentDatabase(), "ReadCurrentUser", storageHashTable(list));
        while (stored.next()) {
            userName = stored.record().field("opt_name_first").value().toString();
        }
        dictionaryDialog->ui->labelUserD->setText(userName);
        stored.finish();

        unsigned d(4);
        QString m_mdate = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), d).data(Qt::DisplayRole).toString();
        dictionaryDialog->ui->labelDateD->setText(m_mdate);
    }
}

void ContractorType::slotDataChanged(const QModelIndex &index)
{
    if (index.isValid()){
        emit selectionModelIndex(index);
    }
}

void ContractorType::slotGetSelectionModel(const QModelIndex &index)
{
    if (index.isValid()){
        m_selectedItem = true;
    }
}

void ContractorType::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (m_rad == RecordActionDatabase::ardInsert) {
        list.append(dictionaryDialog->ui->lineEditItem->text());
        list.append((int)dictionaryDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "InsertPartnerType", storageHashTable(list));
        stored.finish();
    }
    else
    if (m_rad == RecordActionDatabase::ardUpdate) {
        unsigned i(0);
        unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

        list.append(m_code);
        list.append(dictionaryDialog->ui->lineEditItem->text());
        list.append((int)(dictionaryDialog->ui->checkBoxActual->isChecked()));
        stored = execStored(currentDatabase(), "UpdatePartnerType", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(dictionaryDialog);
    m_selectedItem = false;
}

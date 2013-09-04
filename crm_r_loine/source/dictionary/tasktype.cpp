#include "source/dictionary/tasktype.h"
#include "ui_dictionarydialog.h"
#include "source/additionally/communicate.h"

#define  MODEL_COLUMN_COUNT 5

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

TaskType::TaskType(QWidget *parent /* = 0 */):
    DictionaryTemplate(parent)
  , actualRecords(true)
{
    m_model      = new QStandardItemModel(this);

    m_proxymodel = new QSortFilterProxyModel(this);
    m_proxymodel->setSourceModel(m_model);
    m_proxymodel->setFilterKeyColumn(1);
    m_proxymodel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel  = new QItemSelectionModel(m_proxymodel);

    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
    treeView->setModel(m_proxymodel);
    treeView->setSelectionModel(m_selectionModel);

    dictionaryDialog->ui->comboBoxIcon->setEnabled (false);
    dictionaryDialog->ui->comboBoxIcon->setEditable(false);

    ui->m_lblCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(this, SIGNAL(pushSelectRecordData()), SLOT(slotPushSelectRecordData()));
    connect(lineEditSearchToItem, SIGNAL(textEdited(QString)),
            m_proxymodel, SLOT(setFilterWildcard(QString)));
    connect(getClearButton(), SIGNAL(clicked()), SLOT(slotClearSearchToItem()));
    connect(dictionaryDialog, SIGNAL(saveDataChanged()), this,      SLOT(slotInsertOrUpdateRecords()));
    connect(m_selectionModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex)));
    connect(this, SIGNAL(selectionModelIndex(QModelIndex)), SLOT(slotGetSelectionModel(QModelIndex)));

    slotSelectRecords(actualRecords);
}

TaskType::~TaskType()
{
    if (IS_VALID_PTR(m_selectionModel))  { delete m_selectionModel;  m_selectionModel  = nullptr; }
    if (IS_VALID_PTR(m_proxymodel))      { delete m_proxymodel;      m_proxymodel      = nullptr; }
    if (IS_VALID_PTR(m_model))           { delete m_model;           m_model           = nullptr; }
}

void TaskType::fillingModel(QSqlQuery &stored)
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
               item->setIcon(QIcon("data/picture/sidebar/tasktype.ico"));
            }
            m_model->setItem(j ,i, item);
        }
        ++j;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void TaskType::slotSelectRecords(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    list.append((int)actual);
    stored = this->execStored(currentDatabase(), "ReadAllTaskType", storageHashTable(list));

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

void TaskType::slotCopyRecords(void)
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
            stored = execStored(currentDatabase(), "CopyTaskType", storageHashTable(list));
            stored.finish();

            slotRefreshRecords();

        } else if (answer.clickedButton() == m_cancel){
            treeView->clearSelection();
            answer.reject();
        }
    } else {
        Communicate::showing(QString("Не удается копировать,\n запись не выбрана"), 3000);
    }
    m_selectedItem = false;
}


void TaskType::slotDeleteRecords(void)
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
            stored = execStored(currentDatabase(), "DeleteTaskType", storageHashTable(list));
            stored.finish();

            slotRefreshRecords();

        } else if (answer.clickedButton() == m_cancel){
            treeView->clearSelection();
            answer.reject();
        }
    } else {
        Communicate::showing(QString("Не удается удалить,\n запись не выбрана"), 3000);
    }
    m_selectedItem = false;
}

void TaskType::slotRefreshRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    list.append(!actualRecords);
    stored = execStored(currentDatabase(), "ReadAllTaskType", storageHashTable(list));

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

void TaskType::slotPushSelectRecordData(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    QString         userName;

    if (m_rad == RecordActionDatabase::ardInsert) {
        dictionaryDialog->ui->labelUserD->setText(currentUser());
        dictionaryDialog->ui->labelDateD->setText(QString(tr("Не определено")).trimmed());
    }
    else
    if (m_rad == RecordActionDatabase::ardUpdate){
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
        while (stored.next()){
            userName = stored.record().field("opt_name_first").value().toString();
        }
        dictionaryDialog->ui->labelUserD->setText(userName);
        stored.finish();

        unsigned d(4);
        QString m_mdate = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), d).data(Qt::DisplayRole).toString();
        dictionaryDialog->ui->labelDateD->setText(m_mdate);
    }
}

void TaskType::slotDataChanged(const QModelIndex &index)
{
    if (index.isValid()){
       emit selectionModelIndex(index);
    }
}

void TaskType::slotGetSelectionModel(const QModelIndex &index)
{
    if (index.isValid()){
       m_selectedItem = true;
    }
}

void TaskType::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (m_rad == RecordActionDatabase::ardInsert){
        list.append(dictionaryDialog->ui->lineEditItem->text());
        list.append((int)dictionaryDialog->ui->checkBoxActual->isChecked());
        stored = execStored(currentDatabase(), "InsertTaskType", storageHashTable(list));
        stored.finish();
    }
    else
    if (m_rad == RecordActionDatabase::ardUpdate){
        unsigned i(0);
        unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

        list.append(m_code);
        list.append(dictionaryDialog->ui->lineEditItem->text());
        list.append((int)(dictionaryDialog->ui->checkBoxActual->isChecked()));
        stored = execStored(currentDatabase(), "UpdateTaskType", storageHashTable(list));
        stored.finish();
    }
    slotRefreshRecords();
    clearEditDialog(dictionaryDialog);
    m_selectedItem = false;
}

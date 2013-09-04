#include "source/dictionary/priorities.h"
#include "ui_dictionarydialog.h"
#include "source/additionally/communicate.h"

#define  MODEL_COLUMN_COUNT 7

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

Priorities::Priorities(QWidget *parent /* = 0 */):
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

    ui->m_lblCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    dictionaryDialog->ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/red.ico"),    "Высокий");
    dictionaryDialog->ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/yellow.ico"), "Средний");
    dictionaryDialog->ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/green.ico"),  "Низкий");

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

Priorities::~Priorities()
{
    if (IS_VALID_PTR(m_selectionModel))  { delete m_selectionModel;  m_selectionModel  = nullptr; }
    if (IS_VALID_PTR(m_proxymodel))      { delete m_proxymodel;      m_proxymodel      = nullptr; }
    if (IS_VALID_PTR(m_model))           { delete m_model;           m_model           = nullptr; }
}

void Priorities::fillingModel(QSqlQuery &stored)
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
                item->setIcon(QIcon("data/picture/sidebar/priority.ico"));
            } else
            if (i == 5){
                QStandardItem *item = new QStandardItem();
                unsigned m_value = stored.record().field("pt_level").value().toUInt();
                switch (m_value) {
                case 0:
                    item->setIcon(QIcon("data/picture/additionally/red.ico"));
                    break;
                case 1:
                    item->setIcon(QIcon("data/picture/additionally/yellow.ico"));
                    break;
                case 2:
                    item->setIcon(QIcon("data/picture/additionally/green.ico"));
                    break;
                default:
                    break;
                }
                m_model->setItem(j, ncols, item);
            }
            m_model->setItem(j, i, item);
        }
        ++j;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void Priorities::slotSelectRecords(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    list.append((int)actual);
    stored = this->execStored(currentDatabase(), "ReadAllPriorityType", storageHashTable(list));

    fillingModel(stored);
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Иконка"));

    for (int i = 0; i != m_model->columnCount(); ++i){
        if (i == 1 || i == 6) {
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

void Priorities::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    QString         m_name;

    unsigned i(0);
    unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

    if (m_selectionModel->currentIndex().column() != 1){
         m_name = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
    } else {
         m_name = m_selectionModel->currentIndex().data(Qt::DisplayRole).toString();
    }

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
            stored = execStored(currentDatabase(), "CopyPriorityType", storageHashTable(list));
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

void Priorities::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    QString         m_name;

    unsigned i(0);
    unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

    if (m_selectionModel->currentIndex().column() != 1){
         m_name = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
    } else {
         m_name = m_selectionModel->currentIndex().data(Qt::DisplayRole).toString();
    }

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
            stored = execStored(currentDatabase(), "DeletePriorityType", storageHashTable(list));
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

void Priorities::slotRefreshRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    list.append(!actualRecords);
    stored = execStored(currentDatabase(), "ReadAllPriorityType", storageHashTable(list));

    fillingModel(stored);
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Иконка"));

    for (int i = 0; i != m_model->columnCount(); ++i){
        if (i == 1 || i == 6) {
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

void Priorities::slotPushSelectRecordData(void)
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

        unsigned l(5);
        unsigned m_level = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), l).data(Qt::DisplayRole).toUInt();
        dictionaryDialog->ui->comboBoxIcon->setCurrentIndex(m_level);
    }
}

void Priorities::slotDataChanged(const QModelIndex &index)
{
    if (index.isValid()){
        emit selectionModelIndex(index);
    }
}

void Priorities::slotGetSelectionModel(const QModelIndex &index)
{
    if (index.isValid()){
        m_selectedItem = true;
    }
}

void Priorities::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (m_rad == RecordActionDatabase::ardInsert){
        list.append(dictionaryDialog->ui->lineEditItem->text());
        list.append((int)dictionaryDialog->ui->checkBoxActual->isChecked());
        list.append((int)(dictionaryDialog->ui->comboBoxIcon->currentIndex()));
        stored = execStored(currentDatabase(), "InsertPriorityType", storageHashTable(list));
        stored.finish();
    }
    else
    if (m_rad == RecordActionDatabase::ardUpdate){
        unsigned i(0);
        unsigned m_code = m_selectionModel->currentIndex().sibling(m_selectionModel->currentIndex().row(), i).data(Qt::DisplayRole).toUInt();

        list.append(m_code);
        list.append(dictionaryDialog->ui->lineEditItem->text());
        list.append((int)(dictionaryDialog->ui->checkBoxActual->isChecked()));
        list.append((int)(dictionaryDialog->ui->comboBoxIcon->currentIndex()));
        stored = execStored(currentDatabase(), "UpdatePriorityType", storageHashTable(list));
        stored.finish();
   }
   slotRefreshRecords();
   clearEditDialog(dictionaryDialog);
   m_selectedItem = false;
}

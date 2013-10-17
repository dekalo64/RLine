#include "source/crm_dictionary/dct_priorities.h"
#include "source/crm_additionally/adl_communicate.h"
#include "source/crm_dialog/dlg_message.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CPriorities::CPriorities(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(false)
{
// model
    modelPriorities           = new QStandardItemModel(this);
    modelSelectionPriorities  = new QItemSelectionModel(modelPriorities);

    treeCppsst->setRootIsDecorated(false);
    treeCppsst->setAlternatingRowColors(true);
    treeCppsst->setModel(modelPriorities);
    treeCppsst->setSelectionModel(modelSelectionPriorities);
    treeCppsst->installEventFilter(this);

    modelPriorities->insertColumns(0, PRIORITIES_MODEL_COLUMN_COUNT);
    modelPriorities->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    modelPriorities->setHeaderData(3, Qt::Horizontal, QObject::tr("Уровень"));

    QVector<int> storage;
                 storage.append(0);
                 storage.append(2);
    CDictionaryCore::columnHidden(treeCppsst, modelPriorities, storage);
                 storage.clear();

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged(QList<QString>)), this, SLOT(slotInsertOrUpdateRecords(QList<QString>)));
    connect(this, SIGNAL(enabledComboBox(bool)), cppsstDialog, SLOT(slotEnabledComboBox(bool)));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindPriorities(QString)));

    slotFillPriorities();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CPriorities::~CPriorities()
{
    if (IS_VALID_PTR(modelSelectionPriorities))  { delete modelSelectionPriorities;  modelSelectionPriorities  = nullptr; }
    if (IS_VALID_PTR(modelPriorities))           { delete modelPriorities;           modelPriorities           = nullptr; }
}

bool CPriorities::eventFilter(QObject *object, QEvent *event)
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


void CPriorities::slotCreateEditDialog(const QString &action)
{
    if (currentDatabase().isOpen()) {

        QString::compare(action, "add") == 0 ? act = Action::Add : act = Action::Edit;

        cppsstDialog->setWindowTitle(QString("Приоритет"));

        emit enabledComboBox(true);

        if (treeCppsst == focusWidget()){
            if (act == Action::Add){

                QList<QString> param;
                if (fillListSelectedRecord(param)){
                    cppsstDialog->fillFormSelectedRecord(param, act);
                    cppsstDialog->show();
                }
            }else if (act == Action::Edit){
                if (!modelSelectionPriorities->selection().isEmpty()){

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

void CPriorities::slotCreateEditDialog()
{
    slotCreateEditDialog("edit");
}

void CPriorities::fillPrioritiesModel(QSqlQuery &stored)
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

void CPriorities::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        const int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPriorities->selection().isEmpty()){
            CMessage answer(this, "Копирование", "Подтверждаете копирование?");
            QPushButton *copy   = answer.addButton(QString("Копировать"), QMessageBox::ActionRole);
            QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == copy){
                list.append(code);
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "CopyPriorityType", storageHashTable(list));
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

void CPriorities::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        const int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPriorities->selection().isEmpty()) {
            CMessage answer(this, "Удаление", "Подтверждаете удаление?");
            QPushButton *_delete = answer.addButton(QString("Удалить"), QMessageBox::ActionRole);
            QPushButton *cancel = answer.addButton(QString("Отмена"),  QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == _delete){
                list.append(code);
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "DeletePriorityType", storageHashTable(list));
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

void CPriorities::slotRefreshRecords()
{
    slotFillPriorities();
}

bool CPriorities::fillListSelectedRecord(QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (act == Action::Add) {

        param.append(QString("%1").arg(-1));
        param.append("Нет данных");
        param.append("Нет данных");

    } else if (act == Action::Edit) {

        const int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOnePriorityType", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                const QString name = stored.value(stored.record().indexOf("pt_name")).toString();
                const int    level = stored.value(stored.record().indexOf("pt_level")).toUInt();
                const bool  actual = stored.value(stored.record().indexOf("pt_actual")).toBool();
                const QString user = stored.value(stored.record().indexOf("pt_muser")).toString();
                const QString date = stored.value(stored.record().indexOf("pt_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                param.append(name);
                param.append(QVariant(level).toString());
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

void CPriorities::slotInsertOrUpdateRecords(const QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    const int code = modelSelectionPriorities->currentIndex().sibling(modelSelectionPriorities->currentIndex().row(), 0).data().toUInt();

    list.append(code);
    list.append(param.at(0));
    list.append(param.at(2));
    list.append(param.at(1));

    if (act == Action::Add) {
        list.removeAt(0);
        stored = execStored(currentDatabase(), "InsertPriorityType", storageHashTable(list));
    }
    else if (act == Action::Edit) {
        stored = execStored(currentDatabase(), "UpdatePriorityType", storageHashTable(list));
    }
    stored.finish();

    slotRefreshRecords();
    CDictionaryCore::clearEditDialog(cppsstDialog);
}

void CPriorities::slotFillPriorities()
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

void CPriorities::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void CPriorities::slotFindPriorities(const QString &text)
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

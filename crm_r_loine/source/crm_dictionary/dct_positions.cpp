#include "source/crm_dictionary/dct_positions.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CPositions::CPositions(QWidget *parent /* = 0 */):
    CCppsst(parent)
  , actualRecords(false)
{

// model
    modelPositions      = new QStandardItemModel(this);
    modelSelectionPositions  = new QItemSelectionModel(modelPositions);

    treeCppsst->setRootIsDecorated(false);
    treeCppsst->setAlternatingRowColors(true);
    treeCppsst->setModel(modelPositions);
    treeCppsst->setSelectionModel(modelSelectionPositions);
    treeCppsst->installEventFilter(this);

    modelPositions->insertColumns(0, POSITIONS_MODEL_COLUMN_COUNT);
    modelPositions->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> storage;
                 storage.append(0);
                 storage.append(2);
    CDictionaryCore::columnHidden(treeCppsst, modelPositions, storage);
                 storage.clear();

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    connect(cppsstDialog, SIGNAL(saveDataChanged(QList<QString>)), SLOT(slotInsertOrUpdateRecords(QList<QString>)));
    connect(this, SIGNAL(enabledComboBox(bool)), cppsstDialog, SLOT(slotEnabledComboBox(bool)));
    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindPositions(QString)));

    slotFillPositions();

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CPositions::~CPositions()
{
    if (IS_VALID_PTR(modelSelectionPositions))  { delete modelSelectionPositions;  modelSelectionPositions  = nullptr; }
    if (IS_VALID_PTR(modelPositions))           { delete modelPositions;           modelPositions           = nullptr; }
}

bool CPositions::eventFilter(QObject *object, QEvent *event)
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

void CPositions::slotCreateEditDialog(const QString &action)
{
    if (currentDatabase().isOpen()) {

        QString::compare(action, "add") == 0 ? act = Action::Add : act = Action::Edit;

        cppsstDialog->setWindowTitle(QString("Должность"));

        emit enabledComboBox(false);

        if (treeCppsst == focusWidget()){
            if (act == Action::Add){

                QList<QString> param;
                if (fillListSelectedRecord(param)){
                    cppsstDialog->fillFormSelectedRecord(param, act);
                    cppsstDialog->show();
                }
            }else if (act == Action::Edit){
                if (!modelSelectionPositions->selection().isEmpty()){

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

void CPositions::slotCreateEditDialog()
{
    slotCreateEditDialog("edit");
}

void CPositions::fillPositionsModel(QSqlQuery &stored)
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

void CPositions::slotCopyRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        const int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPositions->selection().isEmpty()){
            QMessageBox answer;
                        answer.setText(QString("Подтверждаете копирование?"));
                        answer.setWindowTitle("Копирование");
                        answer.setIcon(QMessageBox::Question);

            QPushButton *copy   = answer.addButton(QString("Копировать"), QMessageBox::ActionRole);
            QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == copy){
                list.append(code);
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "CopyPosition", storageHashTable(list));
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

void CPositions::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        const int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

        if (!modelSelectionPositions->selection().isEmpty()) {
            QMessageBox answer;
                        answer.setText(QString("Подтверждаете удаление?"));
                        answer.setWindowTitle(QString("Удаление"));
                        answer.setIcon(QMessageBox::Question);

            QPushButton *_delete = answer.addButton(QString("Удалить"), QMessageBox::ActionRole);
            QPushButton *cancel = answer.addButton(QString("Отмена"),  QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == _delete){
                list.append(code);
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "DeletePosition", storageHashTable(list));
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

void CPositions::slotRefreshRecords()
{
    slotFillPositions();
}

bool CPositions::fillListSelectedRecord(QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (act == Action::Add) {

        param.append(QString("%1").arg(-1));
        param.append("Нет данных");
        param.append("Нет данных");

    } else if (act == Action::Edit) {

        const int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

        list.append(code);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadOnePosition", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            while (stored.next()) {
                const QString name = stored.value(stored.record().indexOf("pos_name")).toString();
                const bool  actual = stored.value(stored.record().indexOf("pos_actual")).toBool();
                const QString user = stored.value(stored.record().indexOf("pos_muser")).toString();
                const QString date = stored.value(stored.record().indexOf("pos_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

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

void CPositions::slotInsertOrUpdateRecords(const QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    int code = modelSelectionPositions->currentIndex().sibling(modelSelectionPositions->currentIndex().row(), 0).data().toUInt();

    list.append(code);
    list.append(param.at(0));
    list.append(QVariant(param.at(2)).toBool());

    if (act == Action::Add) {
        list.removeAt(0);
        stored = execStored(currentDatabase(), "InsertPosition", storageHashTable(list));
    }
    else if (act == Action::Edit) {
        stored = execStored(currentDatabase(), "UpdatePosition", storageHashTable(list)); 
    }
    stored.finish();

    slotRefreshRecords();
    CDictionaryCore::clearEditDialog(cppsstDialog);
}


void CPositions::slotFillPositions()
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

void CPositions::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

}

void CPositions::slotFindPositions(const QString &text)
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

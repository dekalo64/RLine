#include "ui_dct_countrycity.h"

#include "source/crm_dictionary/dct_countrycity.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CCountryCity::CCountryCity(QWidget *parent  /* = 0 */) :
    QWidget(parent)
  , ui(new Ui::CCountryCity)
{
    ui->setupUi(this);

// models
    modelCountry  = new QStandardItemModel(this);
    modelCity     = new QStandardItemModel(this);

    modelSelectionCountry  = new QItemSelectionModel(modelCountry);
    modelSelectionCity     = new QItemSelectionModel(modelCity);

// dialogs
    countryDialog = new CCountryDialog(this);
    cityDialog    = new CCityDialog   (this);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeCountry = new CCountryTreeView (this);
    treeCity    = new CCityTreeView    (this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
                 hbox->addWidget(treeCountry);
                 hbox->addWidget(treeCity);

    splitter->setLayout(hbox);
    splitter->setCollapsible(0, false);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    ui->hLayoutTreeView->addWidget(splitter);

    treeCountry->setObjectName("treeCountry");
    treeCountry->setModel(modelCountry);
    treeCountry->setMinimumWidth(230);
    treeCountry->setSelectionModel(modelSelectionCountry);
    treeCountry->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeCountry->installEventFilter(this);

    treeCity->setObjectName("treeCity");
    treeCity->setRootIsDecorated(false);
    treeCity->setAlternatingRowColors(true);
    treeCity->setModel(modelCity);
    treeCity->setSelectionModel(modelSelectionCity);
    treeCity->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeCity->installEventFilter(this);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_ ]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    actualRecords = false;
    focusedWidget = nullptr;

    mc.codeCity    = -1;
    mc.nameCity    = QString("");
    mc.codeCountry = -1;
    mc.nameCountry = QString("");

    root = new QStandardItem(QString("Страны и города"));
    root->setIcon(QIcon("data/picture/additionally/root.png"));
    modelCountry->insertColumns(0, COUNTRY_MODEL_COLUMN_COUNT);
    modelCountry->setItem(0, 0, root);

    QFont font(treeCountry->font());
          font.setBold (true);
    modelCountry->setData(modelCountry->index(0, 0), font, Qt::FontRole);
    modelCountry->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> storage;
                 storage.append(1);
                 storage.append(2);
                 storage.append(3);
    CDictionaryCore::columnHidden(treeCountry, modelCountry, storage);
                 storage.clear();

    root->setChild(modelCountry->rowCount(root->index()), new QStandardItem("Загрузка..."));

    modelCity->insertColumns(0, CITY_MODEL_COLUMN_COUNT);

    modelCity->setHeaderData(1, Qt::Horizontal, QObject::tr("Страна"));
    modelCity->setHeaderData(2, Qt::Horizontal, QObject::tr("Название"));
    modelCity->setHeaderData(3, Qt::Horizontal, QObject::tr("Name"));
    modelCity->setHeaderData(4, Qt::Horizontal, QObject::tr("Телефон/Код"));

                 storage.append(0);
    CDictionaryCore::columnHidden(treeCity, modelCity, storage);
                 storage.clear();

    connect(treeCountry, SIGNAL(expanded(QModelIndex)),  SLOT(slotFillGroup(QModelIndex)));
    connect(treeCountry, SIGNAL(collapsed(QModelIndex)), SLOT(slotClearGroup(QModelIndex)));
    connect(treeCountry, SIGNAL(clicked(QModelIndex)),   SLOT(slotFillCities(QModelIndex)));

    connect(countryDialog, SIGNAL(saveDataChanged(QList<QString>)), this, SLOT(slotInsertOrUpdateRecords(QList<QString>)));
    connect(cityDialog,    SIGNAL(saveDataChanged(QList<QString>)), this, SLOT(slotInsertOrUpdateRecords(QList<QString>)));

    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindCities(QString)));

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CCountryCity::~CCountryCity()
{
    if (IS_VALID_PTR(focusedWidget))          { delete focusedWidget;          focusedWidget          = nullptr; }

    if (IS_VALID_PTR(countryDialog))          { delete countryDialog;          countryDialog          = nullptr; }
    if (IS_VALID_PTR(cityDialog))             { delete cityDialog;             cityDialog             = nullptr; }

    if (IS_VALID_PTR(modelSelectionCity))     { delete modelSelectionCity;     modelSelectionCity     = nullptr; }
    if (IS_VALID_PTR(modelSelectionCountry))  { delete modelSelectionCountry;  modelSelectionCountry  = nullptr; }
    if (IS_VALID_PTR(modelCity))              { delete modelCity;              modelCity              = nullptr; }
    if (IS_VALID_PTR(modelCountry))           { delete modelCountry;           modelCountry           = nullptr; }

    if (IS_VALID_PTR(ui))                     { delete ui;                     ui                     = nullptr; }
}

bool CCountryCity::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<CCountryTreeView*>(treeCountry)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();

            for (QAction *action : getContextMenu()->actions()){
                disconnect(action, SIGNAL(triggered()), 0, 0);
            }

            connect(getContextMenu()->actions().at(0), SIGNAL(triggered()), SLOT(slotCreateEditDialog()));
            connect(getContextMenu()->actions().at(2), SIGNAL(triggered()), SLOT(slotDeleteRecords()));
            connect(getContextMenu()->actions().at(3), SIGNAL(triggered()), SLOT(slotRefreshRecordsCountry()));

            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeCountry->clearFocus();
            return false;
        }
    } else if (object == qobject_cast<CCityTreeView*>(treeCity)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();

            for (QAction *action : getContextMenu()->actions()){
                disconnect(action, SIGNAL(triggered()), 0, 0);
            }

            connect(getContextMenu()->actions().at(0), SIGNAL(triggered()), SLOT(slotCreateEditDialog()));
            connect(getContextMenu()->actions().at(2), SIGNAL(triggered()), SLOT(slotCutRecords()));
            connect(getContextMenu()->actions().at(3), SIGNAL(triggered()), SLOT(slotCopyRecords()));
            connect(getContextMenu()->actions().at(4), SIGNAL(triggered()), SLOT(slotPasteRecords()));
            connect(getContextMenu()->actions().at(5), SIGNAL(triggered()), SLOT(slotDeleteRecords()));
            connect(getContextMenu()->actions().at(7), SIGNAL(triggered()), SLOT(slotRefreshRecordsCity()));

            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeCity->clearFocus();
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void CCountryCity::fillCountryModel(QSqlQuery &stored, const QModelIndex &index)
{
    modelCountry->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    QString   it;
    int  ncols = stored.record().count();
    int  row(modelCountry->rowCount(index));

    while(stored.next()){
        if (stored.value(stored.record().indexOf("cty_parent")).toInt() ==
            modelCountry->sibling(index.row(), 1, index).data().toInt()){
                 for (int i = 0; i != ncols; ++i){
                     it = stored.value(i).toString();
                     QStandardItem *item = new QStandardItem(it);
                     if (i == stored.record().indexOf("cty_name_first"))
                            item->setIcon(QIcon("data/picture/additionally/catalog.png"));

                     if (i == stored.record().indexOf("cty_name_first")){
                            modelCountry->itemFromIndex(index)->setChild(row, 0, item);
                     }
                     else if (i == stored.record().indexOf("cty_code")){
                            modelCountry->itemFromIndex(index)->setChild(row, 1, item);
                     }
                     else if (i == stored.record().indexOf("cty_parent")){
                            modelCountry->itemFromIndex(index)->setChild(row, 2, item);
                     }
                     else if (i == stored.record().indexOf("cty_actual")){
                            modelCountry->itemFromIndex(index)->setChild(row, 3, item);
                     }

                     if (item)
                         item->setChild(item->rowCount(), 0, new QStandardItem("Загрузка..."));
                 }
                 ++row;
        }
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CCountryCity::slotFillGroup(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelCountry->hasChildren(index)){
        modelCountry->removeRows(0, modelCountry->rowCount(index), index);
    }

    list.append((int)actualRecords);
    list.append(SKIP); // parameter skip
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllCountries", storageHashTable(list));

    fillCountryModel(stored, index);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CCountryCity::slotFillCities(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (root->index() != index){

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        const int code = index.sibling(index.row(), 1).data().toInt();

        list.append((int)actualRecords);
        list.append(code);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "crm_ReadAllCities", storageHashTable(list));

        fillCityModel(stored);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
    } else {
        modelCity->removeRows(0, modelCity->rowCount(QModelIndex()), QModelIndex());
    }
}

void CCountryCity::slotClearGroup(const QModelIndex &index)
{
    if (index == root->index()){
        if (modelCountry->hasChildren(index)){
            modelCountry->removeRows(1, modelCountry->rowCount(index), index);
        }
    root->setChild(modelCountry->rowCount(root->index()), new QStandardItem("Загрузка..."));
    } else
    if (modelCountry->hasChildren(index)){
            modelCountry->removeRows(1, modelCountry->rowCount(index), index);
    }
    modelCountry->itemFromIndex(index)->setChild(modelCountry->rowCount(index), new QStandardItem("Загрузка..."));
}

void CCountryCity::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void CCountryCity::slotFindCities(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "crm_FindCities", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillCityModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

void CCountryCity::fillCityModel(QSqlQuery &stored)
{
    modelCity->removeRows(0, modelCity->rowCount(QModelIndex()), QModelIndex());
    modelCity->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    int  ncols = stored.record().count();
    int  row(0);

    while(stored.next()){
        for (int i = 0; i != ncols; ++i){
            if (i == stored.record().indexOf("cit_code")){
                modelCity->setItem(row, 0, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("cit_country_name")){
                modelCity->setItem(row, 1, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("cit_name_first")){
                modelCity->setItem(row, 2, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("cit_name_second")){
                modelCity->setItem(row, 3, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("cit_phone_code")){
                modelCity->setItem(row, 4, new QStandardItem(stored.value(i).toString()));
            }
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CCountryCity::slotCutRecords()
{
    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) ||
                        modelCountry->hasChildren(modelSelectionCountry->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено перемещение стран"));
                    return;
                }
            }

            if ((focusedWidget->objectName() == treeCity->objectName())) {
                mc.codeCity   =
                        modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toInt();
                mc.nameCity =
                        modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 2).data().toString();
            } else {
                mc.codeCity  =  -1;
                mc.nameCity  =  "";
            }
        } else
        CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
    CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCountryCity::slotCopyRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            unsigned codeCountry(0);
            unsigned codeCity(0);

            if (focusedWidget->objectName() == treeCity->objectName()){
                codeCountry = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();
                codeCity    = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt();
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (!modelSelectionCountry->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено копирование стран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if (modelSelectionCity->isSelected(modelSelectionCity->currentIndex())){
                    QMessageBox answer;
                                answer.setText(QString("Подтверждаете копирование?"));
                                answer.setWindowTitle(QString("Копирование"));
                                answer.setIcon(QMessageBox::Question);

                    QPushButton *copy   = answer.addButton(QString("Копировать"), QMessageBox::ActionRole);
                    QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                    answer.exec();

                    if (answer.clickedButton() == copy){
                        list.append(codeCity);
                        list.append(codeCountry);
                        stored = execStored(currentDatabase(), "CopyCity", storageHashTable(list));
                        stored.finish();

                        slotRefreshRecordsCity();

                    } else if (answer.clickedButton() == cancel){
                        answer.reject();
                    }
                }
            } else
                CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCountryCity::slotPasteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        mc.codeCountry   =
                modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toInt();
        mc.nameCountry =
                modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 0).data().toString();

        if IS_VALID_PTR(focusedWidget){

            if ((focusedWidget->objectName() == treeCountry->objectName()) ||
                (focusedWidget->objectName() == treeCity->objectName())){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if ((focusedWidget->objectName() == treeCountry->objectName()) ||
                (focusedWidget->objectName() == treeCity->objectName())){
                  if (modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) && mc.codeCity > -1) {

                      QMessageBox answer;
                                  answer.setText(QString("Подтверждаете перемещение?"));
                                  answer.setWindowTitle(QString("Перемещение"));
                                  answer.setIcon(QMessageBox::Question);

                      QPushButton *move   = answer.addButton(QString("Переместить"), QMessageBox::ActionRole);
                      QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                      answer.exec();

                      if (answer.clickedButton() == move){
                          list.append(mc.codeCity);
                          list.append(mc.codeCountry);
                          stored = execStored(currentDatabase(), "MoveCity", storageHashTable(list));
                          stored.finish();

                          slotRefreshRecordsCity();

                          mc.codeCity  =  -1;
                          mc.nameCity  =  "";
                      } else if (answer.clickedButton() == cancel){
                          answer.reject();
                      }
                  }
            } else
                CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCountryCity::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    bool            removable(false);

    unsigned code(0);
    QString  name(QString(""));

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeCountry->objectName()){
                code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();
                name = modelSelectionCountry->currentIndex().data().toString();
            } else if (focusedWidget->objectName() == treeCity->objectName()){
                code = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt();
                name = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 2).data().toString();
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelCountry->hasChildren(modelSelectionCountry->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName() &&
                    modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex())){
                removable = true;
            }
            if (focusedWidget->objectName() == treeCity->objectName() &&
                    modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) &&
                    modelSelectionCity->isSelected(modelSelectionCity->currentIndex())){
                removable = true;
            }
            if (removable){
                QMessageBox answer;
                            answer.setText(QString("Подтверждаете удаление?"));
                            answer.setWindowTitle(QString("Удаление"));
                            answer.setIcon(QMessageBox::Question);

                QPushButton *_delete = answer.addButton(QString("Удалить"), QMessageBox::ActionRole);
                QPushButton *cancel = answer.addButton(QString("Отмена"),  QMessageBox::ActionRole);

                answer.exec();

                if (answer.clickedButton() == _delete){
                    list.append(code);
                    if (focusedWidget->objectName() == treeCountry->objectName()){
                        stored = execStored(currentDatabase(), "DeleteCountry", storageHashTable(list));
                        stored.finish();
                        slotRefreshRecordsCountry();
                    } else if (focusedWidget->objectName() == treeCity->objectName()){
                        stored = execStored(currentDatabase(), "DeleteCity", storageHashTable(list));
                        stored.finish();
                        slotRefreshRecordsCity();
                    }
                } else if (answer.clickedButton() == cancel){
                    answer.reject();
                }
            } else
                CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCountryCity::slotRefreshRecords()
{
    if (focusedWidget->objectName() == treeCountry->objectName()){
           slotRefreshRecordsCountry();
    } else
    if (focusedWidget->objectName() == treeCity->objectName()){
           slotRefreshRecordsCity();
    }
}

void CCountryCity::slotRefreshRecordsCountry()
{
    slotFillGroup(root->index());
}

void CCountryCity::slotRefreshRecordsCity()
{
    slotFillCities(modelSelectionCountry->currentIndex());
}

void CCountryCity::slotCreateEditDialog(const QString &action)
{
    if (currentDatabase().isOpen()) {    

        QString::compare(action, "add") == 0 ? act = Action::Add : act = Action::Edit;

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && act == Action::Edit){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelSelectionCountry->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if(modelSelectionCountry->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && act == Action::Add){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && act == Action::Edit){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCity->objectName()){
                if(!modelSelectionCity->isSelected(modelSelectionCity->currentIndex()) && act == Action::Edit){
                    CCommunicate::showing(QString("Не удается выполнить, город/регион не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelSelectionCountry->currentIndex() != root->index()){

                    QList<QString>  param;
                    if (fillListSelectedRecord(param)){
                        countryDialog->fillFormSelectedRecord(param, act);
                        countryDialog->show();
                    }
                }
            } else if (focusedWidget->objectName() == treeCity->objectName()){

                    QList<QString>  param;
                    if (fillListSelectedRecord(param)){
                        cityDialog->fillFormSelectedRecord(param, act);
                        cityDialog->show();
                    }
              }
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CCountryCity::slotCreateEditDialog(void)
{
    slotCreateEditDialog("edit");
}

bool CCountryCity::fillListSelectedRecord(QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusedWidget->objectName() == treeCountry->objectName()){

        if (act == Action::Add){

            param.append("Нет данных");
            param.append("Нет данных");

        } else if (act == Action::Edit){

            const int code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();

            list.append(code);
            stored.setForwardOnly(true);
            stored = execStored(currentDatabase(), "ReadOneCountry", storageHashTable(list));

            if (stored.numRowsAffected() > 0) {
                while (stored.next()) {
                    const QString namef  = stored.value(stored.record().indexOf("cty_name_first")).toString();
                    const QString namee  = stored.value(stored.record().indexOf("cty_name_second")).toString();
                    const QString phone  = stored.value(stored.record().indexOf("cty_phone_code")).toString();
                    const bool    actual = stored.value(stored.record().indexOf("cty_actual")).toBool();
                    const QString user   = stored.value(stored.record().indexOf("cty_muser")).toString();
                    const QString date   = stored.value(stored.record().indexOf("cty_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                    param.append(namef);
                    param.append(namee);
                    param.append(phone);
                    param.append(QVariant(actual).toString());
                    param.append(user);
                    param.append(date);
                }
            } else {
                CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
                return false;
            }
        }
    } else if (focusedWidget->objectName() == treeCity->objectName()){

        if (act == Action::Add){

            const QString name = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 0).data().toString();
            param.append(name);

            const int code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();

            list.append(code);
            stored.setForwardOnly(true);
            stored = execStored(currentDatabase(), "ReadOneCountry", storageHashTable(list));

            while (stored.next()) {
                const QString code = stored.value(stored.record().indexOf("cty_phone_code")).toString();
                param.append(code);
            }

            param.append("Нет данных");
            param.append("Нет данных");

        } else if (act == Action::Edit){

            const int code = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt();

            list.append(code);
            stored.setForwardOnly(true);
            stored = execStored(currentDatabase(), "ReadOneCity", storageHashTable(list));

            if (stored.numRowsAffected() > 0) {
                while (stored.next()) {

                    const QString namecof  = stored.value(stored.record().indexOf("cty_name_first")).toString();
                    const QString namecif  = stored.value(stored.record().indexOf("cit_name_first")).toString();
                    const QString namee    = stored.value(stored.record().indexOf("cit_name_second")).toString();
                    const QString phonecty = stored.value(stored.record().indexOf("cty_phone_code")).toString();
                    const QString phonecit = stored.value(stored.record().indexOf("cit_phone_code")).toString();
                    const bool    actual   = stored.value(stored.record().indexOf("cit_actual")).toBool();
                    const QString user     = stored.value(stored.record().indexOf("cit_muser")).toString();
                    const QString date     = stored.value(stored.record().indexOf("cit_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                    param.append(namecof);
                    param.append(namecif);
                    param.append(namee);
                    param.append(phonecty);
                    param.append(phonecit);
                    param.append(QVariant(actual).toString());
                    param.append(user);
                    param.append(date);
                }
            } else {
                CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
                return false;
            }
        }
        }
    stored.finish();
    return true;
}

QMenu *CCountryCity::getContextMenu() const
{
    if (focusedWidget->objectName() == treeCountry->objectName()){
        return treeCountry->menu;
    } else if (focusedWidget->objectName() == treeCity->objectName()){
        return treeCity->menu;
    }
    return nullptr;
}

void CCountryCity::slotInsertOrUpdateRecords(const QList<QString> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusedWidget->objectName() == treeCountry->objectName()){
        const int code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();

        list.append(code);
        list.append(param.at(0));
        list.append(param.at(1));
        list.append(param.at(2));
        list.append(QVariant(param.at(3)).toBool());

        if (act == Action::Add){
            stored = execStored(currentDatabase(), "InsertCountry", storageHashTable(list));
        } else if (act == Action::Edit){
            stored = execStored(currentDatabase(), "UpdateCountry", storageHashTable(list));     
        }
        stored.finish();

        slotRefreshRecordsCountry();
        CDictionaryCore::clearEditDialog(countryDialog);
    }
    else if (focusedWidget->objectName() == treeCity->objectName()){

        if (act == Action::Add){
           const int code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();
           list.append(code);
        } else if (act == Action::Edit){
           const int code = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt();
           list.append(code);
        }

        list.append(param.at(0));
        list.append(param.at(1));
        list.append(param.at(2));
        list.append(QVariant(param.at(3)).toBool());

        if (act == Action::Add){
            stored = execStored(currentDatabase(), "InsertCity", storageHashTable(list));
        } else if (act == Action::Edit){
            stored = execStored(currentDatabase(), "UpdateCity", storageHashTable(list));
        }
        stored.finish();

        slotRefreshRecordsCity();
        CDictionaryCore::clearEditDialog(cityDialog);
    }
}

CCityTreeView::CCityTreeView(QWidget *parent) :
    QTreeView(parent)
  , menu(new QMenu(parent))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Вырезать"), this));
    menu->addAction(new QAction(QObject::tr("Копировать"), this));
    menu->addAction(new QAction(QObject::tr("Вставить"), this));
    menu->addAction(new QAction(QObject::tr("Удалить"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Обновить"), this));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(slotCustomContextMenuRequested(const QPoint &)));
}

CCityTreeView::~CCityTreeView()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CCityTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    for (QAction *action : menu->actions()){
            action->setEnabled(true);
    }
    menu->setStyleSheet("QMenu {"
                                "background-color: #f4f4f4;"
                                "margin: 0px;"
                                "border: 1px solid #515151;"
                        "}"
                        "QMenu::item {"
                                "padding: 2px 25px 2px 20px;"
                                "border: 1px solid transparent;"
                        "}"
                        "QMenu::item:selected {"
                                "color: #ffffff;"
                                "background-color: #68a44a;"
                        "}"
                        "QMenu::separator {"
                                "height: 1px;"
                                "background: #aaaaaa;"
                                "margin-left: 10px;"
                                "margin-right: 5px;"
                        "}"
                        "QMenu::indicator {"
                                "width:  10px;"
                                "height: 10px;"
                        "}");
    QModelIndex index = indexAt(pos);
    QVector<int> disable;
    if (index.isValid()){
            disable.append(4);
            disable.append(7);
    } else {
            disable.append(0);
            disable.append(2);
            disable.append(3);
            disable.append(5);
    }
    for (auto i = disable.begin(); i != disable.end(); ++i){
        if (disable.contains(*i)){
            menu->actions().at(*i)->setEnabled(false);
        }
    }
    menu->exec(viewport()->mapToGlobal(pos));
}

void CCityTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        startPosition = event->pos();
    }

    QTreeView::mousePressEvent(event);
}

void CCityTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - startPosition).manhattanLength()
            < QApplication::startDragDistance())
        return;

    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void CCityTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        if (QString::compare(source->objectName(), "editCity") == 0){
            source->setReadOnly(false);

            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
    }
}

void CCityTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        if (QString::compare(source->objectName(), "editCity") == 0){
            source->setReadOnly(false);

            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
    }
}

void CCityTreeView::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCityTreeView::draging()
{
    QModelIndex *item = new QModelIndex(currentIndex());

    if (item->isValid()){
         QMimeData *mimeData = new QMimeData;
                    mimeData->setText(item->data(Qt::DisplayRole).toString());

         QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(QPixmap("data/picture/additionally/dragdrop.png"));

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
         delete item;
    }
}

CCountryTreeView::CCountryTreeView(QWidget *parent) :
    QTreeView(parent)
  , menu(new QMenu(parent))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Удалить"), this));
    menu->addAction(new QAction(QObject::tr("Обновить"), this));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(slotCustomContextMenuRequested(const QPoint &)));
}

CCountryTreeView::~CCountryTreeView()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CCountryTreeView::slotCustomContextMenuRequested(const QPoint &pos)
{
    for (QAction *action : menu->actions()){
            action->setEnabled(true);
    }
    menu->setStyleSheet("QMenu {"
                                "background-color: #f4f4f4;"
                                "margin: 0px;"
                                "border: 1px solid #515151;"
                        "}"
                        "QMenu::item {"
                                "padding: 2px 25px 2px 20px;"
                                "border: 1px solid transparent;"
                        "}"
                        "QMenu::item:selected {"
                                "color: #ffffff;"
                                "background-color: #68a44a;"
                        "}"
                        "QMenu::separator {"
                                "height: 1px;"
                                "background: #aaaaaa;"
                                "margin-left: 10px;"
                                "margin-right: 5px;"
                        "}"
                        "QMenu::indicator {"
                                "width:  10px;"
                                "height: 10px;"
                        "}");
    QModelIndex index = indexAt(pos);
    QVector<int> disable;
    if (index.isValid()){
            disable.append(3);
    } else {
            disable.append(0);
            disable.append(2);
    }
    for (QVector<int>::const_iterator i = disable.begin(); i != disable.end(); ++i){
        if (disable.contains(*i)){
            menu->actions().at(*i)->setEnabled(false);
        }
    }
    menu->exec(viewport()->mapToGlobal(pos));
}

#include "ui_dct_countrycity.h"
#include "ui_dlg_additem.h"

#include "source/crm_dictionary/dct_countrycity.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CCountryCity::CCountryCity(QWidget *parent  /* = 0 */) :
    QWidget(parent)
  , actualRecords(false)
  , ui(new Ui::CCountryCity)
  , cityDialog(new CityDialog(this)), countryDialog(new CountryDialog(this))
  , focusedWidget(nullptr)
  , addItem(new AddItem(this))
{
    ui->setupUi(this);

// model
    modelCountry  = new QStandardItemModel(this);
    modelCity     = new QStandardItemModel(this);

    modelSelectionCountry  = new QItemSelectionModel(modelCountry);
    modelSelectionCity     = new QItemSelectionModel(modelCity);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeViewCountry = new QTreeView   (this);
    treeViewCity    = new CCityTreeView(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
                 hbox->addWidget(treeViewCountry);
                 hbox->addWidget(treeViewCity);

    splitter->setLayout(hbox);
    splitter->setCollapsible(0, false);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    ui->hLayoutTreeView->addWidget(splitter);

    treeViewCountry->setObjectName("treeViewCountry");
    treeViewCountry->setModel(modelCountry);
    treeViewCountry->setMinimumWidth(230);
    treeViewCountry->setSelectionModel(modelSelectionCountry);
    treeViewCountry->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewCountry->installEventFilter(this);

    treeViewCity->setObjectName("treeViewCity");
    treeViewCity->setRootIsDecorated(false);
    treeViewCity->setAlternatingRowColors(true);
    treeViewCity->setModel(modelCity);
    treeViewCity->setSelectionModel(modelSelectionCity);
    treeViewCity->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewCity->installEventFilter(this);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    mc.idCity      = -1;
    mc.nameCity    = QString("");
    mc.idCountry   = -1;
    mc.nameCountry = QString("");

    root = new QStandardItem(QString("Страны и города"));
    root->setIcon(QIcon("data/picture/additionally/root.png"));
    modelCountry->insertColumns(0, COUNTRY_MODEL_COLUMN_COUNT);
    modelCountry->setItem(0, 0, root);

    QFont font(treeViewCountry->font());
          font.setBold (true);
    modelCountry->setData(modelCountry->index(0, 0), font, Qt::FontRole);
    modelCountry->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> vector;
    columnHidden(treeViewCountry, modelCountry, vector << 1 << 2 << 3);
                 vector.clear();

    root->setChild(modelCountry->rowCount(root->index()), new QStandardItem("Загрузка..."));

    modelCity->insertColumns(0, CITY_MODEL_COLUMN_COUNT);

    modelCity->setHeaderData(1, Qt::Horizontal, QObject::tr("Страна"));
    modelCity->setHeaderData(2, Qt::Horizontal, QObject::tr("Название"));
    modelCity->setHeaderData(3, Qt::Horizontal, QObject::tr("Name"));
    modelCity->setHeaderData(4, Qt::Horizontal, QObject::tr("Телефон/Код"));

    columnHidden(treeViewCity, modelCity, vector << 0);
                 vector.clear();

    connect(treeViewCountry, SIGNAL(expanded(QModelIndex)),  SLOT(slotFillGroup(QModelIndex)));
    connect(treeViewCountry, SIGNAL(collapsed(QModelIndex)), SLOT(slotClearGroup(QModelIndex)));
    connect(treeViewCountry, SIGNAL(clicked(QModelIndex)),   SLOT(slotFillCities(QModelIndex)));

    connect(countryDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(cityDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));

    connect(addItem->ui->buttonSave, SIGNAL(clicked()), countryDialog, SLOT(show()));
    connect(addItem->ui->buttonSave, SIGNAL(clicked()), SLOT(slotFillFormSelectedRecord()));
    connect(addItem->ui->buttonSave, SIGNAL(clicked()), addItem, SLOT(close()));

    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindCities(QString)));

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CCountryCity::~CCountryCity()
{
    if (IS_VALID_PTR(focusedWidget))        { delete focusedWidget;            focusedWidget  = nullptr; }
    if (IS_VALID_PTR(addItem))              { delete addItem;                  addItem        = nullptr; }

    if (IS_VALID_PTR(cityDialog))           { delete cityDialog;               cityDialog     = nullptr; }
    if (IS_VALID_PTR(countryDialog))        { delete countryDialog;            countryDialog  = nullptr; }

    if (IS_VALID_PTR(modelSelectionCity))     { delete modelSelectionCity;     modelSelectionCity     = nullptr; }
    if (IS_VALID_PTR(modelSelectionCountry))  { delete modelSelectionCountry;  modelSelectionCountry  = nullptr; }
    if (IS_VALID_PTR(modelCity))              { delete modelCity;              modelCity              = nullptr; }
    if (IS_VALID_PTR(modelCountry))           { delete modelCountry;           modelCountry           = nullptr; }

    if (IS_VALID_PTR(ui))                     { delete ui;                     ui                     = nullptr; }
}

bool CCountryCity::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<QTreeView*>(treeViewCountry)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();
            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeViewCountry->clearFocus();
            return false;
        }
    } else if (object == qobject_cast<CCityTreeView*>(treeViewCity)) {
        if (event->type() == QEvent::FocusIn){
            focusedWidget = focusWidget();
            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeViewCity->clearFocus();
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
    list.append((int)SKIP); // parameter skip
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

        list.append((int)actualRecords);
        list.append((int)index.sibling(index.row(), 1).data().toInt());
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

void CCountryCity::columnHidden(QTreeView *view, QStandardItemModel *model, const QVector<int> &vector)
{
    QVector<int>::const_iterator it = vector.begin();
    while(it != vector.end()){
        for (int i = 0; i != model->columnCount(); ++i){
            if (i == *it)
                 view->setColumnHidden(i, true);
        }
        ++it;
    }
}

void CCountryCity::slotCutRecords()
{
    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                if (modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) ||
                        modelCountry->hasChildren(modelSelectionCountry->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено перемещение стран"));
                    return;
                }
            }

            if ((focusedWidget->objectName() == treeViewCity->objectName())) {
                mc.idCity   =
                        modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toInt();
                mc.nameCity =
                        modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 2).data().toString();
            } else {
                mc.idCity    =  -1;
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

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                codeCountry = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();
                codeCity    = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt();
            }

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                if (!modelSelectionCountry->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, запрещено копирование стран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCity->objectName()){
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

        mc.idCountry   =
                modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toInt();
        mc.nameCountry =
                modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 0).data().toString();

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                  if (modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) &&  mc.idCity > -1) {

                      QMessageBox answer;
                                  answer.setText(QString("Подтверждаете перемещение?"));
                                  answer.setWindowTitle(QString("Перемещение"));
                                  answer.setIcon(QMessageBox::Question);

                      QPushButton *move   = answer.addButton(QString("Переместить"), QMessageBox::ActionRole);
                      QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                      answer.exec();

                      if (answer.clickedButton() == move){
                          list.append(mc.idCity);
                          list.append(mc.idCountry);
                          stored = execStored(currentDatabase(), "MoveCity", storageHashTable(list));
                          stored.finish();
                          slotRefreshRecordsCity();
                          mc.idCity    =  -1;
                          mc.nameCity  =  "";
                      } else if (answer.clickedButton() == cancel){
                          answer.reject();
                      }
                  }
            } else
                CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
        }  else
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

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();
                name = modelSelectionCountry->currentIndex().data().toString();
            } else if (focusedWidget->objectName() == treeViewCity->objectName()){
                code = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt();
                name = modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 2).data().toString();
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                if (modelCountry->hasChildren(modelSelectionCountry->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName() &&
                    modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex())){
                removable = true;
            }
            if (focusedWidget->objectName() == treeViewCity->objectName() &&
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
                    if (focusedWidget->objectName() == treeViewCountry->objectName()){
                        stored = execStored(currentDatabase(), "DeleteCountry", storageHashTable(list));
                        stored.finish();
                        slotRefreshRecordsCountry();
                    } else if (focusedWidget->objectName() == treeViewCity->objectName()){
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
    if (focusedWidget->objectName() == treeViewCountry->objectName()){
            slotRefreshRecordsCountry();
    } else
    if (focusedWidget->objectName() == treeViewCity->objectName()){
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

void CCountryCity::slotCreateEditDialog(const int &r)
{
    if (currentDatabase().isOpen()) {    

        r == 0 ? rad = RecordActionDatabase::ardInsert
               : rad = RecordActionDatabase::ardUpdate;

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                countryDialog->setWindowTitle(QString("Страна"));
            } else if (focusedWidget->objectName() == treeViewCity->objectName()){
                cityDialog->setWindowTitle(QString("Город/Регион"));
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && rad == 1){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCountry->objectName()){
                if (modelSelectionCountry->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

//            if (focusedWidget->objectName() == treeViewCity->objectName()){
//                if(modelSelectionCountry->selection().isEmpty()){
//                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
//                    return;
//                }
//            }

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && rad == 0){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && rad == 1){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeViewCity->objectName()){
                if(!modelSelectionCity->isSelected(modelSelectionCity->currentIndex()) && rad == 1){
                    CCommunicate::showing(QString("Не удается выполнить, город/регион не выбран"));
                    return;
                }
            }

            if (fillFormSelectedRecord()){
                if (focusedWidget->objectName() == treeViewCountry->objectName()){
                    if (rad == 0) {
                        addItem->show();
                        modelSelectionCountry->currentIndex() == root->index() ? addItem->ui->radioNewSubCatalog->setEnabled(false)
                                                                               : addItem->ui->radioNewSubCatalog->setEnabled(true);
                    } else
                        countryDialog->show();
                } else if (focusedWidget->objectName() == treeViewCity->objectName()){
                    cityDialog->show();
                }
            }
        } else
        CCommunicate::showing(QString("Не удается выполнить, страна или город/регион не выбраны"));
    } else
    CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

bool CCountryCity::fillFormSelectedRecord()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusedWidget->objectName() == treeViewCountry->objectName()){
        if (rad == 1){

            list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
            stored.setForwardOnly(true);
            stored = execStored(currentDatabase(), "ReadOneCountry", storageHashTable(list));

            if (stored.numRowsAffected() > 0) {
                while (stored.next()) {
                    countryDialog->setWindowTitle(QString(countryDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("cty_name_first")).toString()));
                    countryDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("cty_name_first")).toString());
                    countryDialog->ui->lineEditNameEng->setText(stored.value(stored.record().indexOf("cty_name_second")).toString());
                    countryDialog->ui->lineEditCityCode->setText(stored.value(stored.record().indexOf("cty_phone_code")).toString());
                    countryDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("cty_actual")).toBool());
                    countryDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("cty_muser")).toString());
                    countryDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("cty_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                }
            } else {
                CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
                return false;
            }
        }
    } else if (focusedWidget->objectName() == treeViewCity->objectName()){
                    cityDialog->ui->lineEditCountry->setText(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 0).data(Qt::DisplayRole).toString());
                    cityDialog->ui->lineEditCountry->setReadOnly(true);

            if (rad == 0){

                list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "ReadOneCountry", storageHashTable(list));

                while (stored.next()) {
                    cityDialog->ui->lineEditCountryCode->setText(stored.value(stored.record().indexOf("cty_phone_code")).toString());
                }

            } else if (rad == 1){

                    list.append(modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt());
                    stored.setForwardOnly(true);
                    stored = execStored(currentDatabase(), "ReadOneCity", storageHashTable(list));


                    if (stored.numRowsAffected() > 0) {
                        while (stored.next()) {
                            cityDialog->setWindowTitle(QString(cityDialog->windowTitle() + " - [ %1 ]").arg(stored.value(stored.record().indexOf("cit_name_first")).toString()));
                            cityDialog->ui->lineEditCountry->setText(stored.value(stored.record().indexOf("cty_name_first")).toString());
                            cityDialog->ui->lineEditName->setText(stored.value(stored.record().indexOf("cit_name_first")).toString());
                            cityDialog->ui->lineEditNameEng->setText(stored.value(stored.record().indexOf("cit_name_second")).toString());
                            cityDialog->ui->lineEditCityCode->setText(stored.value(stored.record().indexOf("cit_phone_code")).toString());
                            cityDialog->ui->lineEditCountryCode->setText(stored.value(stored.record().indexOf("cty_phone_code")).toString());
                            cityDialog->ui->checkBoxActual->setChecked(stored.value(stored.record().indexOf("cit_actual")).toBool());
                            cityDialog->ui->labelUserD->setText(stored.value(stored.record().indexOf("cit_muser")).toString());
                            cityDialog->ui->labelDateD->setText(stored.value(stored.record().indexOf("cit_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
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

void CCountryCity::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusedWidget->objectName() == treeViewCountry->objectName()){
        if (rad == 0){
            if (addItem->ui->radioNewCatalog->isChecked()){
                list.append((int)0);
            } else
            list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
            list.append(countryDialog->ui->lineEditName->text());
            list.append(countryDialog->ui->lineEditNameEng->text());
            list.append(countryDialog->ui->lineEditCityCode->text());
            list.append((int)countryDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertCountry", storageHashTable(list));
            stored.finish();
        } else if (rad == 1){
            list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
            list.append(countryDialog->ui->lineEditName->text());
            list.append(countryDialog->ui->lineEditNameEng->text());
            list.append(countryDialog->ui->lineEditCityCode->text());
            list.append((int)countryDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateCountry", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecordsCountry();
        clearEditDialog(countryDialog);
    }
    else if (focusedWidget->objectName() == treeViewCity->objectName()){
        if (rad == 0){
            list.append(modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt());
            list.append(cityDialog->ui->lineEditName->text());
            list.append(cityDialog->ui->lineEditNameEng->text());
            list.append(cityDialog->ui->lineEditCityCode->text());
            list.append((int)cityDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertCity", storageHashTable(list));
            stored.finish();
        } else if (rad == 1){
            list.append(modelSelectionCity->currentIndex().sibling(modelSelectionCity->currentIndex().row(), 0).data().toUInt());
            list.append(cityDialog->ui->lineEditName->text());
            list.append(cityDialog->ui->lineEditNameEng->text());
            list.append(cityDialog->ui->lineEditCityCode->text());
            list.append((int)cityDialog->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateCity", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecordsCity();
        clearEditDialog(cityDialog);
    }
}

CCityTreeView::CCityTreeView(QWidget *parent) :
    QTreeView(parent)
{
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
    CCityTreeView *source = qobject_cast<CCityTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCityTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    CCityTreeView *source = qobject_cast<CCityTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
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

#include "ui_dct_suppliers.h"

#include "source/crm_dictionary/dct_suppliers.h"
#include "source/crm_additionally/adl_communicate.h"

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CSuppliers::CSuppliers(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::CSuppliers)
{
    ui->setupUi(this);

// model
    modelCountry   = new QStandardItemModel(this);
    modelSuppliers = new QStandardItemModel(this);

    modelSelectionCountry   = new QItemSelectionModel(modelCountry);
    modelSelectionSuppliers = new QItemSelectionModel(modelSuppliers);

// dialog
    countryDialog  = new CSupplier_gDialog(this);
    supplierDialog = new CSupplierDialog  (this);

// create #temporary table
    QString query ("SELECT * INTO #ProducerGroup FROM dbo.ViewProducerGroup");
    QSqlQuery temporary(currentDatabase());
              temporary.exec(query);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeCountry   = new CSCountryTreeView (this);
    treeSuppliers = new CSuppliersTreeView(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
                 hbox->addWidget(treeCountry);
                 hbox->addWidget(treeSuppliers);

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

    treeSuppliers->setObjectName("treeSuppliers");
    treeSuppliers->setRootIsDecorated(false);
    treeSuppliers->setAlternatingRowColors(true);
    treeSuppliers->setModel(modelSuppliers);
    treeSuppliers->setSelectionModel(modelSelectionSuppliers);
    treeSuppliers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeSuppliers->installEventFilter(this);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_ ]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    actualRecords = false;
    focusedWidget = nullptr;

    ms.codeSuppliers = -1;
    ms.nameSuppliers = QString("");
    ms.codeCountry   = -1;
    ms.nameCountry   = QString("");

    root = new QStandardItem(QString("Поставщики"));
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

    modelSuppliers->insertColumns(0, SUPPLIERS_MODEL_COLUMN_COUNT);
    modelSuppliers->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование поставщика"));

                 storage.append(0);
                 storage.append(2);
    CDictionaryCore::columnHidden(treeSuppliers, modelSuppliers, storage);
                 storage.clear();

    connect(treeCountry, SIGNAL(expanded(QModelIndex)),  SLOT(slotFillGroup(QModelIndex)));
    connect(treeCountry, SIGNAL(collapsed(QModelIndex)), SLOT(slotClearGroup(QModelIndex)));
    connect(treeCountry, SIGNAL(clicked(QModelIndex)),   SLOT(slotFillSuppliers(QModelIndex)));

    connect(countryDialog, SIGNAL(saveDataChanged(QList<QString>)), this, SLOT(slotInsertOrUpdateRecords(QList<QString>)));
    connect(supplierDialog, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));

    connect(filter, SIGNAL(textChanged(QString)), SLOT(slotFindSuppliers(QString)));

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

CSuppliers::~CSuppliers()
{
// drop #temporary table
    QString query ("DROP TABLE #ProducerGroup");
    QSqlQuery temporary(currentDatabase());
              temporary.exec(query);

    if (IS_VALID_PTR(focusedWidget))          { delete focusedWidget;            focusedWidget            = nullptr; }

    if (IS_VALID_PTR(countryDialog))          { delete countryDialog;            countryDialog            = nullptr; }
    if (IS_VALID_PTR(supplierDialog))         { delete supplierDialog;           supplierDialog           = nullptr; }

    if (IS_VALID_PTR(modelSelectionCountry))  { delete modelSelectionCountry;     modelSelectionCountry   = nullptr; }
    if (IS_VALID_PTR(modelSelectionSuppliers)){ delete modelSelectionSuppliers;   modelSelectionSuppliers = nullptr; }
    if (IS_VALID_PTR(modelCountry))           { delete modelCountry;              modelCountry            = nullptr; }
    if (IS_VALID_PTR(modelSuppliers))         { delete modelSuppliers;            modelSuppliers          = nullptr; }

    if (IS_VALID_PTR(ui))                     { delete ui;                        ui                      = nullptr; }
}

QMenu *CSuppliers::getContextMenu() const
{
    if (focusedWidget->objectName() == treeCountry->objectName()){
        return treeCountry->menu;
    } else if (focusedWidget->objectName() == treeSuppliers->objectName()){
        return treeSuppliers->menu;
    }
    return nullptr;
}

bool CSuppliers::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<CSCountryTreeView*>(treeCountry)) {
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
    } else if (object == qobject_cast<CSuppliersTreeView*>(treeSuppliers)) {
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
            connect(getContextMenu()->actions().at(7), SIGNAL(triggered()), SLOT(slotRefreshRecordsSuppliers()));

            return false;
        } else if (event->type() == QEvent::FocusOut){
            treeSuppliers->clearFocus();
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void CSuppliers::fillCountryModel(QSqlQuery &stored, const QModelIndex &index)
{
    modelCountry->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    QString   it;
    int  ncols = stored.record().count();
    int  row(modelCountry->rowCount(index));

    if (stored.record().field(stored.record().indexOf("grp_parent")).isValid()){
    while(stored.next()){
        if (stored.value(stored.record().indexOf("grp_parent")).toInt() ==
            modelCountry->sibling(index.row(), 1, index).data().toInt()){
                 for (int i = 0; i != ncols; ++i){
                     it = stored.value(i).toString();
                     QStandardItem *item = new QStandardItem(it);
                     if (i == stored.record().indexOf("grp_name"))
                            item->setIcon(QIcon("data/picture/additionally/catalog.png"));

                     if (i == stored.record().indexOf("grp_name")){
                            modelCountry->itemFromIndex(index)->setChild(row, 0, item);
                     }
                     else if (i == stored.record().indexOf("grp_code")){
                            modelCountry->itemFromIndex(index)->setChild(row, 1, item);
                     }
                     else if (i == stored.record().indexOf("grp_parent")){
                            modelCountry->itemFromIndex(index)->setChild(row, 2, item);
                     }
                     else if (i == stored.record().indexOf("grp_actual")){
                            modelCountry->itemFromIndex(index)->setChild(row, 3, item);
                     }

                     if (item)
                         item->setChild(item->rowCount(), 0, new QStandardItem("Загрузка..."));
                 }
                 ++row;
        }
    }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CSuppliers::fillSuppliersModel(QSqlQuery &stored)
{
    modelSuppliers->removeRows(0, modelSuppliers->rowCount(QModelIndex()), QModelIndex());
    modelSuppliers->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    int  ncols = stored.record().count();
    int  row(0);

    while(stored.next()){
        for (int i = 0; i != ncols; ++i){
            if (i == stored.record().indexOf("par_code")){
                modelSuppliers->setItem(row, 0, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_name_first")){
                modelSuppliers->setItem(row, 1, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_actual")){
                modelSuppliers->setItem(row, 2, new QStandardItem(stored.value(i).toString()));
            }
        }
        ++row;
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}
 // work here
bool CSuppliers::fillListSelectedRecord(QList<QVariant> &param)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusedWidget->objectName() == treeCountry->objectName()){

        if (act == Action::Add){

            param.append("Нет данных");
            param.append("Нет данных");

        } else if (act == Action::Edit){

            const int code = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();

            list.append(GROUP_TYPE_SUPPLIERS);
            list.append(code);
            stored.setForwardOnly(true);
            stored = execStored(currentDatabase(), "ReadOneReferenceGroup", storageHashTable(list));

            if (stored.numRowsAffected() > 0) {
                while (stored.next()) {
                    const QString name = stored.value(stored.record().indexOf("grp_name")).toString();
                    const bool  actual = stored.value(stored.record().indexOf("grp_actual")).toBool();
                    const QString user = stored.value(stored.record().indexOf("grp_muser")).toString();
                    const QString date = stored.value(stored.record().indexOf("grp_mdate")).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

                    param.append(name);
                    param.append(QVariant(actual).toString());
                    param.append(user);
                    param.append(date);
                }
            }
         } else {
             CCommunicate::showing(QString("Не удается выполнить, документ либо его элемент был удален другим пользователем"));
             return false;
           }

    } else if (focusedWidget->objectName() == treeSuppliers->objectName()){

            if (act == Action::Add){

                stored.setForwardOnly(true);
                stored = execStored(currentDatabase(), "CreateSupplier", storageHashTable(list));

                while (stored.next()) {
                    const int clientNumber = stored.value(stored.record().indexOf("sup_number")).toUInt();
                    param.append(QVariant(clientNumber).toString());
                }

// create #temporary table
                {
                    QString   query ("SELECT * INTO #TrustedUser FROM dbo.ViewTrustedUser");
                    QSqlQuery temporary(currentDatabase());
                              temporary.exec(query);
                }

                {
                    QSqlQuery  stored;
                               stored.setForwardOnly(true);
                    stored = execStored(currentDatabase(), "ReadOptions", storageHashTable());

                    while (stored.next()) {
                        const QString country    = stored.value(stored.record().indexOf("cty_name_first")).toString();
                        const QString city       = stored.value(stored.record().indexOf("cit_name_first")).toString();
                        const short   currency   = stored.value(stored.record().indexOf("opt_currency")).toUInt();
                        const QString inventory  = stored.value(stored.record().indexOf("pot_inv_name")).toString();
                        const short   language   = stored.value(stored.record().indexOf("opt_language")).toUInt();
                        const float   prepayment = stored.value(stored.record().indexOf("opt_pay_limit")).toFloat();

                        param.append(city);
                        param.append(country);
                        param.append(QVariant(currency).toString());   // int
                        param.append(inventory);
                        param.append(QVariant(language).toString());   // int
                        param.append(QVariant(prepayment).toString()); // float
                    }
                }
                        param.append("Нет данных");
                        param.append("Нет данных");

// drop #temporary table
                {
                    QString   query ("DROP TABLE #TrustedUser");
                    QSqlQuery temporary(currentDatabase());
                              temporary.exec(query);
                }
            } else if (act == Action::Edit){

                    const int code = modelSelectionSuppliers->currentIndex().sibling(modelSelectionSuppliers->currentIndex().row(), 0).data().toUInt();

                    list.append(code);
                    stored.setForwardOnly(true);
                    stored = execStored(currentDatabase(), "ReadOneSupplier", storageHashTable(list));

                    short rows;

                    while (stored.next()) rows++;

                    if (rows > 0) {
                        stored.clear();
                        stored = execStored(currentDatabase(), "ReadOneSupplier", storageHashTable(list));
                        while (stored.next()) {
                             const QString nameS      = stored.value(stored.record().indexOf("par_name_first")).toString();
                             const QString nameE      = stored.value(stored.record().indexOf("par_name_second")).toString();
                             const short   clNumber   = stored.value(stored.record().indexOf("par_client_N")).toUInt();
                             const QString city       = stored.value(stored.record().indexOf("cit_name_first")).toString();
                             const QString country    = stored.value(stored.record().indexOf("cty_name_first")).toString();
                             const QString address    = stored.value(stored.record().indexOf("par_address")).toString();
                             const QString attender   = stored.value(stored.record().indexOf("par_attender")).toString();
                             const QString phoneM     = stored.value(stored.record().indexOf("par_Mob_Phone")).toString();
                             const QString ctyphcode  = stored.value(stored.record().indexOf("cty_phone_code")).toString();
                             const QString citphcode  = stored.value(stored.record().indexOf("cit_phone_code")).toString();
                             const QString phoneR     = stored.value(stored.record().indexOf("par_phone")).toString();
                             const QString fax        = stored.value(stored.record().indexOf("par_fax")).toString();
                             const QString indexP     = stored.value(stored.record().indexOf("par_postal_index")).toString();
                             const QString dateOrd    = stored.value(stored.record().indexOf("ord_date")).toString();
                             const QString email      = stored.value(stored.record().indexOf("par_EMail")).toString();
                             const QString web        = stored.value(stored.record().indexOf("par_Web")).toString();
                             const QString director   = stored.value(stored.record().indexOf("par_Director")).toString();
                             const short   currency   = stored.value(stored.record().indexOf("par_currency")).toUInt();
                             const QString nameI      = stored.value(stored.record().indexOf("inv_name")).toString();
                             const short   language   = stored.value(stored.record().indexOf("par_language")).toUInt();
                             const QString index      = stored.value(stored.record().indexOf("par_index")).toString();
                             const QString balance    = stored.value(stored.record().indexOf("par_balance")).toString();
                             const QString inn        = stored.value(stored.record().indexOf("par_INN")).toString();
                             const QString nds        = stored.value(stored.record().indexOf("par_Certific_NDS")).toString();
                             const QString mfo        = stored.value(stored.record().indexOf("par_MFO")).toString();
                             const QDate   dateIss    = stored.value(stored.record().indexOf("par_Date_Out")).toDate();
                             const QString account    = stored.value(stored.record().indexOf("par_account")).toString();
                             const QString bank       = stored.value(stored.record().indexOf("par_bank")).toString();
                             const QString iban       = stored.value(stored.record().indexOf("par_IBAN")).toString();
                             const QString comment    = stored.value(stored.record().indexOf("par_comment")).toString();
                             const float   service    = stored.value(stored.record().indexOf("par_mark_up")).toFloat();
                             const float   goods      = stored.value(stored.record().indexOf("par_discount")).toFloat();
                             const short   delivery   = stored.value(stored.record().indexOf("par_delivery")).toUInt();
                             const float   prepayment = stored.value(stored.record().indexOf("par_pay_limit")).toFloat();

                             param.append(nameS);
                             param.append(nameE);
                             param.append(clNumber);
                             param.append(city);
                             param.append(country);
                             param.append(address);
                             param.append(attender);
                             param.append(phoneM);
                             param.append(ctyphcode);
                             param.append(citphcode);
                             param.append(phoneR);
                             param.append(fax);
                             param.append(indexP);
                             param.append(dateOrd);
                             param.append(email);
                             param.append(nameS);
                             param.append(web);
                             param.append(director);
                             param.append(currency);
                             param.append(nameI);
                             param.append(language);
                             param.append(index);
                             param.append(balance);
                             param.append(inn);
                             param.append(nds);
                             param.append(mfo);
                             param.append(dateIss);
                             param.append(account);
                             param.append(bank);
                             param.append(iban);
                             param.append(comment);
                             param.append(service);
                             param.append(goods);
                             param.append(delivery);
                             param.append(prepayment);

                             {
                                 // list of producers belonging to suppliers
                                 QSqlQuery stored;
                                           stored.setForwardOnly(true);
                                 stored = execStored(currentDatabase(), "ReadProducerByGroup", storageHashTable());

                                 QHash<QString, QVariant> producer;

                                 QString key, value;

                                 while (stored.next()){
                                     key   = stored.value(stored.record().indexOf("prg_prod_code")).toString();
                                     value = stored.value(stored.record().indexOf("par_name_first")).toString();

                                     producer.insert(key, QVariant(value).toString());
                                 }
                                 param.append(QVariant(producer));
                             }

                             const bool    actual     = stored.value(stored.record().indexOf("par_actual")).toBool();
                             const QString user       = stored.value(stored.record().indexOf("par_muser")).toString();
                             const QString date       = stored.value(stored.record().indexOf("par_mdate")).toString();

                             param.append(actual);
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

void CSuppliers::slotFillGroup(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelCountry->hasChildren(index)){
        modelCountry->removeRows(0, modelCountry->rowCount(index), index);
    }

    list.append(GROUP_TYPE_SUPPLIERS);
    list.append((int)actualRecords);
    list.append((int)SKIP); // parameter skip
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllReferenceGroups", storageHashTable(list));

    fillCountryModel(stored, index);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CSuppliers::slotFillSuppliers(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (root->index() != index){

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append((int)index.sibling(index.row(), 1).data().toInt());
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadAllSuppliers", storageHashTable(list));

        fillSuppliersModel(stored);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
    } else {
        modelSuppliers->removeRows(0, modelSuppliers->rowCount(QModelIndex()), QModelIndex());
    }
}

void CSuppliers::slotClearGroup(const QModelIndex &index)
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

void CSuppliers::slotActualRecords(const bool &actual)
{
    actualRecords = !actual;
    slotRefreshRecords();
    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           : ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void CSuppliers::slotFindSuppliers(const QString &text)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

        list.append((int)actualRecords);
        list.append(text);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "FindSuppliers", storageHashTable(list));

        if (stored.numRowsAffected() > 0) {
            fillSuppliersModel(stored);
        }

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        stored.finish();
}

void CSuppliers::slotCutRecords()
{
    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
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

            if ((focusedWidget->objectName() == treeSuppliers->objectName())) {
                ms.codeSuppliers   =
                        modelSelectionSuppliers->currentIndex().sibling(modelSelectionSuppliers->currentIndex().row(), 0).data().toInt();
                ms.nameSuppliers =
                        modelSelectionSuppliers->currentIndex().sibling(modelSelectionSuppliers->currentIndex().row(), 1).data().toString();
            } else {
                ms.codeSuppliers  =  -1;
                ms.nameSuppliers  =  "";
            }
        } else
        CCommunicate::showing(QString("Не удается выполнить, страна или поставщик не выбраны"));
    } else
    CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));

}

void CSuppliers::slotCopyRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        if IS_VALID_PTR(focusedWidget){

            unsigned codeCountry(0);
            unsigned codeSupplier(0);

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                codeCountry  = modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toUInt();
                codeSupplier = modelSelectionSuppliers->currentIndex().sibling(modelSelectionSuppliers->currentIndex().row(), 0).data().toUInt();
            }

            if (focusedWidget->objectName() == modelSelectionSuppliers->objectName()){
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

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                if (modelSelectionSuppliers->isSelected(modelSelectionSuppliers->currentIndex())){
                    QMessageBox answer;
                                answer.setText(QString("Подтверждаете копирование?"));
                                answer.setWindowTitle(QString("Копирование"));
                                answer.setIcon(QMessageBox::Question);

                    QPushButton *copy   = answer.addButton(QString("Копировать"), QMessageBox::ActionRole);
                    QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                    answer.exec();

                    if (answer.clickedButton() == copy){
                        list.append(codeSupplier);
                        list.append(codeCountry);
                        stored = execStored(currentDatabase(), "CopySupplier", storageHashTable(list));
                        stored.finish();

                        slotRefreshRecordsSuppliers();

                    } else if (answer.clickedButton() == cancel){
                        answer.reject();
                    }
                }
            } else
                CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или поставщик не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CSuppliers::slotPasteRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (currentDatabase().isOpen()) {

        ms.codeCountry   =
                modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 1).data().toInt();
        ms.nameCountry =
                modelSelectionCountry->currentIndex().sibling(modelSelectionCountry->currentIndex().row(), 0).data().toString();

        if IS_VALID_PTR(focusedWidget){

            if ((focusedWidget->objectName() == treeCountry->objectName()) ||
                (focusedWidget->objectName() == treeSuppliers->objectName())){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if ((focusedWidget->objectName() == treeCountry->objectName()) ||
                (focusedWidget->objectName() == treeSuppliers->objectName())){
                  if (modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) && ms.codeSuppliers > -1) {

                      QMessageBox answer;
                                  answer.setText(QString("Подтверждаете перемещение?"));
                                  answer.setWindowTitle(QString("Перемещение"));
                                  answer.setIcon(QMessageBox::Question);

                      QPushButton *move   = answer.addButton(QString("Переместить"), QMessageBox::ActionRole);
                      QPushButton *cancel = answer.addButton(QString("Отмена"),     QMessageBox::ActionRole);

                      answer.exec();

                      if (answer.clickedButton() == move){
                          list.append(ms.codeSuppliers);
                          list.append(ms.codeCountry);
                          stored = execStored(currentDatabase(), "MoveSupplier", storageHashTable(list));
                          stored.finish();

                          slotRefreshRecordsSuppliers();

                          ms.codeSuppliers =  -1;
                          ms.nameSuppliers =  "";
                      } else if (answer.clickedButton() == cancel){
                          answer.reject();
                      }
                  }
            } else
                CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
        }  else
             CCommunicate::showing(QString("Не удается выполнить, страна не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));

}

void CSuppliers::slotDeleteRecords()
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
            } else if (focusedWidget->objectName() == treeSuppliers->objectName()){
                code = modelSelectionSuppliers->currentIndex().sibling(modelSelectionSuppliers->currentIndex().row(), 0).data().toUInt();
                name = modelSelectionSuppliers->currentIndex().sibling(modelSelectionSuppliers->currentIndex().row(), 1).data().toString();
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelCountry->hasChildren(modelSelectionCountry->currentIndex())){
                    CCommunicate::showing(QString("Не удается выполнить, корневая запись"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName() &&
                    modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex())){
                removable = true;
            }
            if (focusedWidget->objectName() == treeSuppliers->objectName() &&
                    modelSelectionCountry->isSelected(modelSelectionCountry->currentIndex()) &&
                    modelSelectionSuppliers->isSelected(modelSelectionSuppliers->currentIndex())){
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
                    if (focusedWidget->objectName() == treeCountry->objectName()){
                        list.append(GROUP_TYPE_SUPPLIERS);
                        list.append(code);

                        stored = execStored(currentDatabase(), "DeleteReferenceGroup", storageHashTable(list));
                        stored.finish();
                        slotRefreshRecordsCountry();
                    } else if (focusedWidget->objectName() == treeSuppliers->objectName()){
                        list.append(code);

                        stored = execStored(currentDatabase(), "DeleteSupplier", storageHashTable(list));
                        stored.finish();
                        slotRefreshRecordsSuppliers();
                    }
                } else if (answer.clickedButton() == cancel){
                    answer.reject();
                }
            } else
                CCommunicate::showing(QString("Не удается выполнить, запись не выбрана"));
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или поставщик не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CSuppliers::slotRefreshRecords()
{
    if (focusedWidget->objectName() == treeCountry->objectName()){
            slotRefreshRecordsCountry();
    } else
    if (focusedWidget->objectName() == treeSuppliers->objectName()){
           slotRefreshRecordsSuppliers();
    }
}

void CSuppliers::slotRefreshRecordsCountry()
{
    slotFillGroup(root->index());
}

void CSuppliers::slotRefreshRecordsSuppliers()
{
    slotFillSuppliers(modelSelectionCountry->currentIndex());
}

void CSuppliers::slotCreateEditDialog(const QString &action)
{
    if (currentDatabase().isOpen()) {

        QString::compare(action, "add") == 0 ? act = Action::Add : act = Action::Edit;

        if IS_VALID_PTR(focusedWidget){

            if (focusedWidget->objectName() == treeCountry->objectName()){
                countryDialog->setWindowTitle(QString("Папка"));
            } else if (focusedWidget->objectName() == treeSuppliers->objectName()){
                supplierDialog->setWindowTitle(QString("Поставщик"));
            }

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

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                if(modelSelectionCountry->selection().isEmpty()){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && act == Action::Add){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                if (modelSelectionCountry->currentIndex() == root->index() && act == Action::Edit){
                    CCommunicate::showing(QString("Не удается выполнить, страна не выбрана"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeSuppliers->objectName()){
                if(!modelSelectionSuppliers->isSelected(modelSelectionSuppliers->currentIndex()) && act == Action::Edit){
                    CCommunicate::showing(QString("Не удается выполнить, поставщик не выбран"));
                    return;
                }
            }

            if (focusedWidget->objectName() == treeCountry->objectName()){
                if (modelSelectionCountry->currentIndex() != root->index()){

                    QList<QVariant>  param;
                    if (fillListSelectedRecord(param)){
                        countryDialog->fillFormSelectedRecord(param, act);
                        countryDialog->show();
                    }
                }
            } else if (focusedWidget->objectName() == treeSuppliers->objectName()){

                    QList<QVariant>  param;
                    if (fillListSelectedRecord(param)){
                        supplierDialog->fillFormSelectedRecord(param, act);
                        supplierDialog->show();
                    }
            }
        } else
            CCommunicate::showing(QString("Не удается выполнить, страна или поставщик не выбраны"));
    } else
        CCommunicate::showing(QString("Не удается выполнить, база данных не доступна"));
}

void CSuppliers::slotCreateEditDialog()
{
    slotCreateEditDialog("edit");
}

// work here
void CSuppliers::slotInsertOrUpdateRecords(const QList<QString> &param)
{
}

CSCountryTreeView::CSCountryTreeView(QWidget *parent) :
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

CSCountryTreeView::~CSCountryTreeView()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CSCountryTreeView::slotCustomContextMenuRequested(const QPoint &pos)
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

CSuppliersTreeView::CSuppliersTreeView(QWidget *parent) :
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

CSuppliersTreeView::~CSuppliersTreeView()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CSuppliersTreeView::slotCustomContextMenuRequested(const QPoint &pos)
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
    for (QVector<int>::const_iterator i = disable.begin(); i != disable.end(); ++i){
        if (disable.contains(*i)){
            menu->actions().at(*i)->setEnabled(false);
        }
    }
    menu->exec(viewport()->mapToGlobal(pos));
}

void CSuppliersTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        startPosition = event->pos();
    }

    QTreeView::mousePressEvent(event);
}

void CSuppliersTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void CSuppliersTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    CSuppliersTreeView *source = qobject_cast<CSuppliersTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CSuppliersTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    CSuppliersTreeView *source = qobject_cast<CSuppliersTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CSuppliersTreeView::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}


void CSuppliersTreeView::draging()
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

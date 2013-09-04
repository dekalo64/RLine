#include "source/dictionary/countryandcity.h"
#include "ui_countryandcity.h"
#include "ui_newitemdialog.h"
#include "source/additionally/communicate.h"

#define  COUNTRY_MODEL_COLUMN_COUNT 6
#define  CITY_MODEL_COLUMN_COUNT    4

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

CountryAndCity::CountryAndCity(QWidget *parent  /* = 0 */) :
    QWidget(parent)
  , actualRecords(false)
  , ui(new Ui::CountryAndCity)
  , editDialogCity(new CityDialog(this))
  , editDialogCountry(new CountryDialog(this))
  , lastFocusedWidget(nullptr)
  , newItem(new NewItemDialog(this))
{
    ui->setupUi(this);

    m_countryModel  = new QStandardItemModel(this);
    m_cityModel     = new QStandardItemModel(this);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_cityModel);
    m_proxyModel->setFilterKeyColumn(1);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionCountryModel  = new QItemSelectionModel(m_countryModel);
    m_selectionCityModel     = new QItemSelectionModel(m_proxyModel);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    treeViewCountry = new QTreeView   (this);
    treeViewCity    = new TreeViewCity(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
                 hbox->addWidget(treeViewCountry);
                 hbox->addWidget(treeViewCity);

    splitter->setLayout(hbox);
    splitter->setCollapsible(0, false);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    ui->hLayoutTreeView->addWidget(splitter);

    treeViewCountry->setObjectName("treeViewCty");
    treeViewCountry->setModel(m_countryModel);
    treeViewCountry->setMinimumWidth(230);
    treeViewCountry->setSelectionModel(m_selectionCountryModel);
    treeViewCountry->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewCountry->installEventFilter(this);

    treeViewCity->setObjectName("treeViewCit");
    treeViewCity->setRootIsDecorated(false);
    treeViewCity->setAlternatingRowColors(true);
    treeViewCity->setModel(m_proxyModel);
    treeViewCity->setSelectionModel(m_selectionCityModel);
    treeViewCity->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewCity->installEventFilter(this);

    lineEditSearchToItem = new LineEditCountryAndCity(this);
    lineEditSearchToItem->setObjectName("lineEditSearchToItem");
    lineEditSearchToItem->setPlaceholderText("Введите наименование");
    ui->hLayoutSearchToItem->addWidget(lineEditSearchToItem);

    lineEditSearchToItem->installEventFilter(this);
    lineEditSearchToItem->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_]+")), this));

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    mc.idCity      = -1;
    mc.nameCity    = QString("");
    mc.idCountry   = -1;
    mc.nameCountry = QString("");

    connect(this, SIGNAL(filterWildcard(QString)),
            m_proxyModel, SLOT(setFilterWildcard(QString)));
    connect(this, SIGNAL(pushSelectRecordData()), this, SLOT(slotPushSelectRecordData()));
    connect(lineEditSearchToItem, SIGNAL(textEdited(QString)),
            m_proxyModel, SLOT(setFilterWildcard(QString)));
    connect(getClearButton(), SIGNAL(clicked()), SLOT(slotClearSearchToItem()));
    connect(treeViewCountry, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotDataChanged(QModelIndex)));
    connect(editDialogCountry, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(editDialogCity, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
    connect(newItem->ui->buttonSave, SIGNAL(clicked()), editDialogCountry, SLOT(show()));
    connect(newItem->ui->buttonSave, SIGNAL(clicked()), SLOT(slotPushSelectRecordData()));
    connect(newItem->ui->buttonSave, SIGNAL(clicked()), newItem, SLOT(close()));

    slotSelectRecords(actualRecords);
}

CountryAndCity::~CountryAndCity()
{
    if (IS_VALID_PTR(lastFocusedWidget))        { delete lastFocusedWidget;        lastFocusedWidget        = nullptr; }
    if (IS_VALID_PTR(newItem))                  { delete newItem;                  newItem                  = nullptr; }
    if (IS_VALID_PTR(editDialogCity))           { delete editDialogCity;           editDialogCity           = nullptr; }
    if (IS_VALID_PTR(editDialogCountry))        { delete editDialogCountry;        editDialogCountry        = nullptr; }
    if (IS_VALID_PTR(m_selectionCityModel))     { delete m_selectionCityModel;     m_selectionCityModel     = nullptr; }
    if (IS_VALID_PTR(m_selectionCountryModel))  { delete m_selectionCountryModel;  m_selectionCountryModel  = nullptr; }
    if (IS_VALID_PTR(m_proxyModel))             { delete m_proxyModel;             m_proxyModel             = nullptr; }
    if (IS_VALID_PTR(m_cityModel))              { delete m_cityModel;              m_cityModel              = nullptr; }
    if (IS_VALID_PTR(m_countryModel))           { delete m_countryModel;           m_countryModel           = nullptr; }
    if (IS_VALID_PTR(ui))                       { delete ui;                       ui                       = nullptr; }
}

QToolButton *CountryAndCity::getClearButton() const
{
    return lineEditSearchToItem->clearButton;
}

bool CountryAndCity::eventFilter(QObject *object, QEvent *event)
{
    if (object == qobject_cast<LineEditCountryAndCity*>(lineEditSearchToItem)) {
        if (event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyboardCapture(keyEvent)){
                m_proxyModel->setFilterKeyColumn(2);
                return false;
            }
        }
        if (event->type() == QEvent::KeyRelease){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyboardCapture(keyEvent)){
                m_proxyModel->setFilterKeyColumn(2);
                return false;
            }
        }
    }
    else
    if (object == qobject_cast<QTreeView*>(treeViewCountry)) {
        if (event->type() == QEvent::FocusIn){
            lastFocusedWidget = focusWidget();
            m_proxyModel->setFilterKeyColumn(1);
            return false;
        } else
        if (event->type() == QEvent::FocusOut){
            treeViewCountry->clearFocus();
            return false;
        }
    }
    else
    if (object == qobject_cast<TreeViewCity*>(treeViewCity)) {
        if (event->type() == QEvent::FocusIn){
            lastFocusedWidget = focusWidget();
            return false;
        } else
        if (event->type() == QEvent::FocusOut){
            treeViewCity->clearFocus();
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

void CountryAndCity::fillingCountryModel(QSqlQuery &stored)
{
    QMultiMap<int, QString>      map_np;
    QMultiMap<QString, QString>  map_nn;
    QList <QStandardItem *>      list;

    m_countryModel->clear();
    m_countryModel->insertColumns(0, COUNTRY_MODEL_COLUMN_COUNT);
    m_countryModel->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    QStandardItem *parentItem = new QStandardItem(QString("Страны и города").trimmed());
    parentItem->setIcon(QIcon("data/picture/additionally/root.png"));
    m_countryModel->setItem(0, parentItem);
    QFont m_font(treeViewCountry->font());
    m_font.setBold (true);
    m_countryModel->setData(m_countryModel->index(0, 0), m_font, Qt::FontRole);

    map_np = setTreeData (stored, parentItem); // parent : name
    map_nn = namePerIndex(stored, map_np);

    for (QMultiMap<QString, QString>::const_iterator i = map_nn.begin(); i != map_nn.end(); ++i){
        if (list.size() > 0) {
            list.clear();
        }
        list = m_countryModel->findItems(i.value(), Qt::MatchExactly | Qt::MatchRecursive);
        QStandardItem *p;
        if (list.size() > 0){
            foreach (p, list) {
                stored.first();
                while(stored.next()){
                    if (stored.value(stored.record().indexOf("cty_parent")).toInt() != 0 &&
                            (stored.value(stored.record().indexOf("cty_name_first")).toString() == i.key())){
                        setTreeValue(stored, p, p->rowCount());
                    }
                }
            }
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

QMultiMap<int, QString> CountryAndCity::setTreeData(QSqlQuery &stored, QStandardItem *p)
{
    QMultiMap<int, QString> map;

    while(stored.next()){
        if (stored.value(stored.record().indexOf("cty_parent")).toInt() == 0){
            setTreeValue(stored, p, p->rowCount());
        } else {
            map.insert(
                        stored.value(stored.record().indexOf("cty_parent")).toInt(),
                        stored.value(stored.record().indexOf("cty_name_first")).toString());
        }
    }
  return map;
}

void CountryAndCity::setTreeValue(QSqlQuery &stored, QStandardItem *p, int row)
{
    unsigned ncols = stored.record().count();

    for (unsigned i = 0; i != ncols; ++i){
        switch (i) {
        case 0:
            p->setChild(row, 1, new QStandardItem(stored.value(i).toString()));
            break;
        case 1:
            p->setChild(row, 5, new QStandardItem(stored.value(i).toString()));
            break;
        case 2:
            p->setChild(row, 0, new QStandardItem(QIcon("data/picture/additionally/catalog.png"), stored.value(i).toString()));
            break;
        case 3:
            p->setChild(row, 2, new QStandardItem(stored.value(i).toString()));
            break;
        case 4:
            p->setChild(row, 3, new QStandardItem(stored.value(i).toString()));
            break;
        case 5:
            p->setChild(row, 4, new QStandardItem(stored.value(i).toString()));
            break;
        default:
            break;
        }
    }
}

QMultiMap<QString, QString> CountryAndCity::namePerIndex(QSqlQuery &stored, QMultiMap<int, QString> &m)
{
    QMultiMap<QString, QString> map;

    stored.first();
    while (stored.next()){
        for (QMultiMap<int, QString>::const_iterator i = m.begin(); i != m.end(); ++i){
            if (stored.value(stored.record().indexOf("cty_code")).toInt() == i.key()){
                map.insert(i.value(), stored.value(stored.record().indexOf("cty_name_first")).toString()); // name : parent name
            }
        }
    }
    return map;
}

void CountryAndCity::slotSelectRecordsCountry(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    list.append((int)actual);
    list.append((int)0); // параметр skip
    stored.setForwardOnly(false);
    stored = execStored(currentDatabase(), "ReadAllCountries", storageHashTable(list));

    fillingCountryModel(stored);
    m_countryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    for (int i = 0; i != m_countryModel->columnCount(); ++i){
        if (i != 0) {
            treeViewCountry->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();

    actual ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           :
             ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void CountryAndCity::fillingCityModel(QSqlQuery &stored)
{
    m_cityModel->clear();
    m_cityModel->insertColumns(0, CITY_MODEL_COLUMN_COUNT);
    m_cityModel->insertRows(stored.numRowsAffected(), 0);

    unsigned  j(0);

    unsigned ncols = stored.record().count();
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    stored.first();
    while(stored.next())
    {
        for (unsigned i = 0; i != ncols; ++i){
            QStandardItem *item = new QStandardItem(stored.value(i).toString().trimmed());
            m_cityModel->setItem(j ,i, item);
        }
        ++j;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

bool CountryAndCity::keyboardCapture(QKeyEvent *event) const
{
    if (((event->nativeVirtualKey() >= 48) && (event->nativeVirtualKey() <= 57))
            ||
       (((event->nativeVirtualKey() >= 65) && (event->nativeVirtualKey() <= 90)))
            ||
        ((event->nativeVirtualKey() >= 97) && (event->nativeVirtualKey() <= 122))) {
        return true;
    }
    return false;
}

void CountryAndCity::slotSelectRecordsCity(bool actual)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    list.append((int)actual);
    list.append(QVariant::Invalid);
    stored.setForwardOnly(false);
    stored = execStored(currentDatabase(), "ReadAllCities", storageHashTable(list));

    fillingCityModel(stored);
    m_cityModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Страна"));
    m_cityModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Название"));
    m_cityModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Name"));
    m_cityModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Телефон/Код"));

    for (int i = 0; i != m_cityModel->columnCount(); ++i){
        if (i <= 0 || i >= 5) {
            treeViewCity->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();

    actual ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           :
             ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));
}

void CountryAndCity::slotCutRecords()
{
    if IS_VALID_PTR(lastFocusedWidget){

        if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
            if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) &&
                    qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается выполнить,\n страна не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
            if (m_selectionCountryModel->isSelected(m_selectionCountryModel->currentIndex()) ||
                    m_countryModel->hasChildren(m_selectionCountryModel->currentIndex())){
                Communicate::showing(QString("Не удается выполнить,\n запрещено перемещение стран"), 3000);
                return;
            }
        }

        if ((lastFocusedWidget->objectName() == treeViewCity->objectName())) {
            mc.idCity   =
                    m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 0).data(Qt::DisplayRole).toInt();
            mc.nameCity =
                    m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 2).data(Qt::DisplayRole).toString();
        } else {
            mc.idCity    =  -1;
            mc.nameCity  =  "";
        }
    } else
    Communicate::showing(QString("Не удается выполнить,\n выберете страну и город"), 3000);
}

void CountryAndCity::slotCopyRecords()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    unsigned m_codeCty(0);
    unsigned m_codeCit(0);
    QString  m_name(QString().trimmed());

    if IS_VALID_PTR(lastFocusedWidget){

        if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
              m_codeCty = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt();
              m_codeCit = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt();
              m_name    = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 2).data(Qt::DisplayRole).toString();
        }

        if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
            if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) &&
                qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается копировать,\n страна не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
            if (m_selectionCountryModel->isSelected(m_selectionCountryModel->currentIndex()) ||
                m_countryModel->hasChildren(m_selectionCountryModel->currentIndex())){
                Communicate::showing(QString("Не удается копировать,\n запрещено копирование стран"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
            if (m_selectionCityModel->isSelected(m_selectionCityModel->currentIndex())){
                QMessageBox answer;
                            answer.setText(QString(tr("Подтверждаете копирование <b>%1</b>?").arg(m_name)));
                            answer.setWindowTitle(QString(tr("Копирование")));
                            answer.setIcon(QMessageBox::Question);

                QPushButton *m_copy   = answer.addButton(QString(tr("Копировать")), QMessageBox::ActionRole);
                QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

                answer.exec();

                if (answer.clickedButton() == m_copy){
                    list.append(m_codeCit);
                    list.append(m_codeCty);
                    stored = execStored(currentDatabase(), "CopyCity", storageHashTable(list));
                    stored.finish();
                } else
                if (answer.clickedButton() == m_cancel){
                    answer.reject();
                }
            slotRefreshRecordsCity();
            emit filterWildcard(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString());
          } else {
                Communicate::showing(QString("Не удается копировать,\n запись не выбрана"), 3000);
            }
        }
    } else {
        Communicate::showing(QString("Не удается копировать,\n запись не выбрана"), 3000);
    }
}

void CountryAndCity::slotPasteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    mc.idCountry   =
                m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 1).data(Qt::DisplayRole).toInt();
    mc.nameCountry =
                m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 0).data(Qt::DisplayRole).toString();

    if IS_VALID_PTR(lastFocusedWidget){

        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
            if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) &&
                    qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается выполнить,\n страна не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()             &&
            m_selectionCountryModel->isSelected(m_selectionCountryModel->currentIndex()) &&  mc.idCity > -1) {

            QMessageBox answer;
                        answer.setText(QString(tr("Подтверждаете перемещение <b>%1</b> в папку <b>%2</b>?").arg(mc.nameCity).arg(mc.nameCountry)));
                        answer.setWindowTitle(QString(tr("Перемещение")));
                        answer.setIcon(QMessageBox::Question);

            QPushButton *m_move   = answer.addButton(QString(tr("Переместить")), QMessageBox::ActionRole);
            QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),     QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == m_move){
                list.append(mc.idCity);
                list.append(mc.idCountry);
                stored = execStored(currentDatabase(), "MoveCity", storageHashTable(list));
                stored.finish();

                mc.idCity    =  -1;
                mc.nameCity  =  "";
            } else
            if (answer.clickedButton() == m_cancel){
                answer.reject();
            }
        slotSelectRecordsCity(actualRecords);
        emit filterWildcard(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString());
      } else {
            Communicate::showing(QString("Не удается переместить,\n запись не выбрана"), 3000);
        }
    }
}

void CountryAndCity::slotDeleteRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    bool            removable(false);

    unsigned m_code(0);
    QString  m_name(QString().trimmed());

    if IS_VALID_PTR(lastFocusedWidget){
        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
            m_code = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt();
            m_name = m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString();
        } else
        if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
            m_code = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt();
            m_name = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 2).data(Qt::DisplayRole).toString();
        }

        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
            if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex())){
                Communicate::showing(QString("Не удается удалить,\n корневая запись"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
            if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) &&
                qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                Communicate::showing(QString("Не удается удалить,\n страна не выбрана"), 3000);
                return;
            }
        }

        if (lastFocusedWidget->objectName() == treeViewCountry->objectName()       &&
            m_selectionCountryModel->isSelected(m_selectionCountryModel->currentIndex())){
                 removable = true;
        }
        if (lastFocusedWidget->objectName() == treeViewCity->objectName()          &&
            m_selectionCountryModel->isSelected(m_selectionCountryModel->currentIndex())   &&
            m_selectionCityModel->isSelected(m_selectionCityModel->currentIndex()))      {
                 removable = true;
        }
        if (removable){
            QMessageBox answer;
                        answer.setText(QString(tr("Подтверждаете удаление <b>%1</b>?").arg(m_name)));
                        answer.setWindowTitle(QString(tr("Удаление")));
                        answer.setIcon(QMessageBox::Question);

            QPushButton *m_delete = answer.addButton(QString(tr("Удалить")), QMessageBox::ActionRole);
            QPushButton *m_cancel = answer.addButton(QString(tr("Отмена")),  QMessageBox::ActionRole);

            answer.exec();

            if (answer.clickedButton() == m_delete){
                list.append(m_code);
                if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
                    stored = execStored(currentDatabase(), "DeleteCountry", storageHashTable(list));
                    stored.finish();
                    slotRefreshRecordsCountry();
                    treeViewCountry->expandToDepth(0);
                } else
                if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
                    stored = execStored(currentDatabase(), "DeleteCity", storageHashTable(list));
                    stored.finish();
                    slotRefreshRecordsCity();
                    emit filterWildcard(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().trimmed());
                }
            } else
                if (answer.clickedButton() == m_cancel){
                    answer.reject();
                }
        } else {
            Communicate::showing(QString("Не удается удалить,\n запись не выбрана"), 3000);
        }
    } else {
        Communicate::showing(QString("Не удается удалить,\n запись не выбрана"), 3000);
    }
}

void CountryAndCity::slotRefreshRecords()
{
    if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
            slotRefreshRecordsCountry();
    } else
    if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
           slotRefreshRecordsCity();
    }
}

void CountryAndCity::slotRefreshRecordsCountry()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    list.append(!actualRecords);
    list.append((int)0); // параметр skip
    stored.setForwardOnly(false);
    stored = execStored(currentDatabase(), "ReadAllCountries", storageHashTable(list));

    fillingCountryModel(stored);
    m_countryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    for (int i = 0; i != m_countryModel->columnCount(); ++i){
        if (i != 0) {
            treeViewCountry->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CountryAndCity::slotRefreshRecordsCity()
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif
    list.append(!actualRecords);
    stored.setForwardOnly(false);
    stored = execStored(currentDatabase(), "ReadAllCity", storageHashTable(list));

    fillingCityModel(stored);
    m_cityModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Страна"));
    m_cityModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Название"));
    m_cityModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Name"));
    m_cityModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Телефон/Код"));

    for (int i = 0; i != m_cityModel->columnCount(); ++i){
        if (i <= 0 || i >= 5) {
            treeViewCity->setColumnHidden(i, true);
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CountryAndCity::slotCreateEditDialog(int r)
{
    if (currentDatabase().isOpen()) {    
        switch (r) {
        case 0:
             m_rad = RecordActionDatabase::ardInsert;
            break;
        case 1:
            m_rad = RecordActionDatabase::ardUpdate;
            break;
        default:
            break;
        }
        if IS_VALID_PTR(lastFocusedWidget){

            if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
                editDialogCountry->setWindowTitle(QString("Страна"));
            } else
            if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
                editDialogCity->setWindowTitle(QString("Город/Регион"));
            }

            if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
                if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) && m_rad == RecordActionDatabase::ardUpdate &&
                    qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                    Communicate::showing(QString("Не удается править,\n корневая запись"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
                if (m_selectionCountryModel->selection().isEmpty()){
                    Communicate::showing(QString("Не удается выполнить,\nстрана не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
                if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) && (m_rad == RecordActionDatabase::ardInsert) &&
                    qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                    Communicate::showing(QString("Не удается добавить,\n страна не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
                if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) && (m_rad == RecordActionDatabase::ardUpdate) &&
                    qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                    Communicate::showing(QString("Не удается править,\n страна не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
                if(!m_selectionCityModel->isSelected(m_selectionCityModel->currentIndex()) && m_rad == RecordActionDatabase::ardUpdate){
                    Communicate::showing(QString("Не удается править,\n запись не выбрана"), 3000);
                    return;
                }
            }

            if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
                if (m_rad == RecordActionDatabase::ardInsert) {
                    newItem->show();
                    if (m_countryModel->hasChildren(m_selectionCountryModel->currentIndex()) &&
                        qstrcmp(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
                        newItem->ui->radioNewSubCatalog->setEnabled(false);
                    } else
                        newItem->ui->radioNewSubCatalog->setEnabled(true);

                } else {
                    editDialogCountry->show();
                }
            } else
                if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
                    editDialogCity->show();
                }
            emit pushSelectRecordData();
        } else{
            Communicate::showing(QString("Не удается выполнить,\nзапись не выбрана"), 3000);
        }
    }
}

void CountryAndCity::slotDataChanged(const QModelIndex &index)
{
    if (index.isValid() && qstrcmp(index.data(Qt::DisplayRole).toString().toUtf8().constData(), QString("Страны и города").trimmed().toUtf8().constData()) == 0){
        emit filterWildcard(QString().trimmed());
    } else
    if (index.isValid()){
        emit filterWildcard(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString());
    }
}

void CountryAndCity::slotSelectRecords(bool actual)
{
    slotSelectRecordsCountry(actual);
    slotSelectRecordsCity(actual);
    actualRecords = !actualRecords;
}

void CountryAndCity::slotPushSelectRecordData()
{
    if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
        if (m_rad == RecordActionDatabase::ardUpdate){
            QString ctyFirstName = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 0).data(Qt::DisplayRole).toString();
            editDialogCountry->setWindowTitle(QString(editDialogCountry->windowTitle() + " запись - [ %1 ]").arg(ctyFirstName));
            editDialogCountry->ui->lineEditName->setText(QString(ctyFirstName).trimmed());

            QString ctySecondName = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 2).data(Qt::DisplayRole).toString();
            editDialogCountry->ui->lineEditNameEng->setText(ctySecondName);

            QString ctyCityCode = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 3).data(Qt::DisplayRole).toString();
            editDialogCountry->ui->lineEditCityCode->setText(ctyCityCode);

            unsigned actual = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 4).data(Qt::DisplayRole).toUInt();
            editDialogCountry->ui->checkBoxActual->setChecked(actual);
        }
    } else
    if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
        editDialogCity->ui->lineEditCountry->setText(m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 0).data(Qt::DisplayRole).toString());
        editDialogCity->ui->lineEditCountry->setReadOnly(true);

        if (m_rad == RecordActionDatabase::ardInsert){
            QString ctyCityCode = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 3).data(Qt::DisplayRole).toString();
            editDialogCity->ui->lineEditCountryCode->setText(ctyCityCode);
        } else
        if (m_rad == RecordActionDatabase::ardUpdate){
            QString ctyFirstName = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 1).data(Qt::DisplayRole).toString();
            editDialogCity->ui->lineEditCountry->setText(ctyFirstName);

            QString citFirstName = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 2).data(Qt::DisplayRole).toString();
            editDialogCity->setWindowTitle(QString(editDialogCity->windowTitle() + " запись - [ %1 ]").arg(citFirstName));
            editDialogCity->ui->lineEditName->setText(QString(citFirstName).trimmed());

            QString citSecondName = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 3).data(Qt::DisplayRole).toString();
            editDialogCity->ui->lineEditNameEng->setText(citSecondName);

            QString citCityCode = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 4).data(Qt::DisplayRole).toString();
            editDialogCity->ui->lineEditCityCode->setText(citCityCode);

            QString ctyCityCode = m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 3).data(Qt::DisplayRole).toString();
            editDialogCity->ui->lineEditCountryCode->setText(ctyCityCode);

            unsigned actual = m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 5).data(Qt::DisplayRole).toUInt();
            editDialogCity->ui->checkBoxActual->setChecked(actual);
        }
    }

}

void CountryAndCity::slotInsertOrUpdateRecords(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (lastFocusedWidget->objectName() == treeViewCountry->objectName()){
        if (m_rad == RecordActionDatabase::ardInsert){
            if (newItem->ui->radioNewCatalog->isChecked()){
                list.append((int)0);
            } else
            list.append(m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt());
            list.append(editDialogCountry->ui->lineEditName->text());
            list.append(editDialogCountry->ui->lineEditNameEng->text());
            list.append(editDialogCountry->ui->lineEditCityCode->text());
            list.append((int)editDialogCountry->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertCountry", storageHashTable(list));
            stored.finish();
        } else
        if (m_rad == RecordActionDatabase::ardUpdate){
            list.append(m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt());
            list.append(editDialogCountry->ui->lineEditName->text());
            list.append(editDialogCountry->ui->lineEditNameEng->text());
            list.append(editDialogCountry->ui->lineEditCityCode->text());
            list.append((int)editDialogCountry->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateCountry", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecordsCountry();
        treeViewCountry->expandToDepth(0);

        clearEditDialog(editDialogCountry);
    }
    else
    if (lastFocusedWidget->objectName() == treeViewCity->objectName()){
        if (m_rad == RecordActionDatabase::ardInsert){
            list.append(m_selectionCountryModel->currentIndex().sibling(m_selectionCountryModel->currentIndex().row(), 1).data(Qt::DisplayRole).toUInt());
            list.append(editDialogCity->ui->lineEditName->text());
            list.append(editDialogCity->ui->lineEditNameEng->text());
            list.append(editDialogCity->ui->lineEditCityCode->text());
            list.append((int)editDialogCity->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "InsertCity", storageHashTable(list));
            stored.finish();
        } else
        if (m_rad == RecordActionDatabase::ardUpdate){
            list.append(m_selectionCityModel->currentIndex().sibling(m_selectionCityModel->currentIndex().row(), 0).data(Qt::DisplayRole).toUInt());
            list.append(editDialogCity->ui->lineEditName->text());
            list.append(editDialogCity->ui->lineEditNameEng->text());
            list.append(editDialogCity->ui->lineEditCityCode->text());
            list.append((int)editDialogCity->ui->checkBoxActual->isChecked());
            stored = execStored(currentDatabase(), "UpdateCity", storageHashTable(list));
            stored.finish();
        }
        slotRefreshRecordsCity();
        emit filterWildcard(m_selectionCountryModel->currentIndex().data(Qt::DisplayRole).toString());

        clearEditDialog(editDialogCity);
    }
}

void CountryAndCity::slotClearSearchToItem(void)
{
    lineEditSearchToItem->undo();
    lineEditSearchToItem->backspace();
}

LineEditCountryAndCity::LineEditCountryAndCity(QWidget *parent)
   : QLineEdit(parent)
{
   clearButton = new QToolButton(this);
   QPixmap pixmapClear("data/picture/additionally/clear-button.png");

   searchButton = new QToolButton(this);
   QPixmap pixmapSearch("data/picture/additionally/search.png");

   clearButton->setIcon(QIcon(pixmapClear));
   clearButton->setIconSize(pixmapClear.size());
   clearButton->setCursor(Qt::PointingHandCursor);
   clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
   clearButton->hide();

   searchButton->setIcon(QIcon(pixmapSearch));
   searchButton->setIconSize(pixmapSearch.size());
   searchButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");

   connect(this, SIGNAL(textChanged(const QString&)), SLOT(slotUpdateCloseButton(const QString&)));

   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth + 1));
   setStyleSheet(QString("QLineEdit { padding-left: %1px; } ").arg(searchButton->sizeHint().width() + frameWidth + 1));
   QSize msz = minimumSizeHint();
   setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                  qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
   setMinimumSize(qMax(msz.width(), searchButton->sizeHint().height() + frameWidth * 2 + 2),
                  qMax(msz.height(), searchButton->sizeHint().height() + frameWidth * 2 + 2));
}

void LineEditCountryAndCity::resizeEvent(QResizeEvent *)
{
   QSize szClear = clearButton->sizeHint();
   QSize szSearch = searchButton->sizeHint();
   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   clearButton->move(rect().right() - frameWidth - szClear.width(),
                     (rect().bottom() + 1 - szClear.height()) / 2);
   searchButton->move(rect().left() + 4,
                     (rect().bottom() + 1 - szSearch.height()) / 2);
}

void LineEditCountryAndCity::slotUpdateCloseButton(const QString& text)
{
    clearButton->setVisible(!text.isEmpty());
}


TreeViewCity::TreeViewCity(QWidget *parent) :
    QTreeView(parent)
{
    setAcceptDrops(true);
}

void TreeViewCity::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void TreeViewCity::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void TreeViewCity::dragEnterEvent(QDragEnterEvent *event)
{
    TreeViewCity *source = qobject_cast<TreeViewCity*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeViewCity::dragMoveEvent(QDragMoveEvent *event)
{
    TreeViewCity *source = qobject_cast<TreeViewCity*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeViewCity::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){

        QMessageBox::information(0, "title", "test");

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeViewCity::draging()
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

#include "source/dictionary/customer.h"
#include "ui_customer.h"
#include <stdio.h>

CCustomer::CCustomer(QWidget *parent) :
    QWidget(parent)
  , actualRecords(false)
  , ui(new Ui::CCustomer)
{
    ui->setupUi(this);

// model
    modelFaces    = new QStandardItemModel(this);
    modelPartner  = new QStandardItemModel(this);
    modelHuman    = new QStandardItemModel(this);

    modelSelectionFaces   = new QItemSelectionModel(modelFaces);
    modelSelectionPartner = new QItemSelectionModel(modelPartner);
    modelSelectionHuman   = new QItemSelectionModel(modelHuman);

// create #temporary table
    QString query ("SELECT * INTO #GroupCustomerDiscounts FROM ViewGroupCustomerDiscounts"
                   "SELECT * INTO #CustomerSubdiller FROM ViewCustomerSubdiller");
    QSqlQuery temporary(currentDatabase());
    temporary.exec(query);

    QSplitter *hSplitter = new QSplitter(Qt::Horizontal);
    QSplitter *vSplitter = new QSplitter(Qt::Vertical);

    QWidget *w1 = new QWidget(this);
    QWidget *w2 = new QWidget(this);

    treeViewFaces          = new QTreeView (this);
    treeViewPartner        = new CCustomerTreeView (this);
    textEditPartnerComment = new QTextEdit (this);
    textEditPartnerComment->setMaximumWidth(100);
    textEditPartnerComment->setReadOnly(true);
    treeViewHuman          = new CCustomerTreeView (this);
    textEditHumanComment   = new QTextEdit (this);
    textEditHumanComment->setMaximumWidth(100);
    textEditHumanComment->setReadOnly(true);

    QHBoxLayout *hboxPartner = new QHBoxLayout(w1);
                 hboxPartner->setMargin(0);
                 hboxPartner->addWidget(treeViewPartner);
                 hboxPartner->addWidget(textEditPartnerComment);

    QHBoxLayout *hboxHuman = new QHBoxLayout(w2);
                 hboxHuman->setMargin(0);
                 hboxHuman->addWidget(treeViewHuman);
                 hboxHuman->addWidget(textEditHumanComment);

    vSplitter->addWidget(w1);
    vSplitter->addWidget(w2);
    vSplitter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    ui->vLayoutUnionPartnerHuman->addWidget(vSplitter);

    hSplitter->addWidget(treeViewFaces);
    hSplitter->addWidget(vSplitter);
    hSplitter->setStretchFactor(1, 3);
    hSplitter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    ui->hLayoutUnionViews->addWidget(hSplitter);

    treeViewFaces->setObjectName("treeViewFaces");
    treeViewFaces->setModel(modelFaces);
    treeViewFaces->setSelectionModel(modelSelectionFaces);
    treeViewFaces->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewFaces->installEventFilter(this);

    treeViewPartner->setObjectName("treeViewPartner");
    treeViewPartner->setRootIsDecorated(false);
    treeViewPartner->setAlternatingRowColors(true);
    treeViewPartner->setModel(modelPartner);
    treeViewPartner->setSelectionModel(modelSelectionPartner);
    treeViewPartner->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewPartner->installEventFilter(this);

    treeViewHuman->setObjectName("treeViewHuman");
    treeViewHuman->setRootIsDecorated(false);
    treeViewHuman->setAlternatingRowColors(true);
    treeViewHuman->setModel(modelHuman);
    treeViewHuman->setSelectionModel(modelSelectionHuman);
    treeViewHuman->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewHuman->installEventFilter(this);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);

    QToolButton *telephone = new QToolButton(this);
    QPixmap pixmapTelephone("data/picture/additionally/telephone.png");

    telephone->setIcon(QIcon(pixmapTelephone));
    telephone->setIconSize(QSize(24, 24));
    telephone->setCursor(Qt::PointingHandCursor);
    telephone->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(telephone);

    QToolButton *meeting = new QToolButton(this);
    QPixmap pixmapMeeting("data/picture/additionally/meeting.png");

    meeting->setIcon(QIcon(pixmapMeeting));
    meeting->setIconSize(QSize(24, 24));
    meeting->setCursor(Qt::PointingHandCursor);
    meeting->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(meeting);

    QToolButton *event = new QToolButton(this);
    QPixmap pixmapEvent("data/picture/additionally/event.png");

    event->setIcon(QIcon(pixmapEvent));
    event->setIconSize(QSize(24, 24));
    event->setCursor(Qt::PointingHandCursor);
    event->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(event);

    QToolButton *task = new QToolButton(this);
    QPixmap pixmapTask("data/picture/additionally/task.png");

    task->setIcon(QIcon(pixmapTask));
    task->setIconSize(QSize(24, 24));
    task->setCursor(Qt::PointingHandCursor);
    task->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    ui->hLayoutSearchToItem->addWidget(task);

    ui->labelCurrentUser->setText(QString("Пользователь: <b><u>" + currentUser() + "</u></b>"));

    root = new QStandardItem(QString("Заказчики"));
    root->setIcon(QIcon("data/picture/additionally/root.png"));
    modelFaces->insertColumns(0, FACES_MODEL_COLUMN_COUNT);
    modelFaces->setItem(0, 0, root);

    QFont font(treeViewFaces->font());
          font.setBold (true);
    modelFaces->setData(modelFaces->index(0, 0), font, Qt::FontRole);
    modelFaces->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));

    QVector<int> vector;
    vector.append(0);
    columnHidden(vector);

    root->setChild(modelFaces->rowCount(root->index()), new QStandardItem("Загрузка..."));

    modelPartner->insertColumns(0, PARTNER_MODEL_COLUMN_COUNT);

    modelPartner->setHeaderData(0, Qt::Horizontal, "Контрагенты");
    modelPartner->setHeaderData(1, Qt::Horizontal, "Тип");
    modelPartner->setHeaderData(2, Qt::Horizontal, "Статус");
    modelPartner->setHeaderData(3, Qt::Horizontal, "Город");
    modelPartner->setHeaderData(4, Qt::Horizontal, "Web");
    modelPartner->setHeaderData(5, Qt::Horizontal, "Руководитель");

    actualRecords
           ? ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Актуальные</u></b>")))
           :
             ui->labelViewState->setText(QString(tr("Отображаются записи: <b><u>Все</u></b>")));

//    connect(this, SIGNAL(pushSelectRecordData()), this, SLOT(slotPushSelectRecordData()));


//    connect(filter, SIGNAL(textEdited(QString)),
//            modelPartner, SLOT()); // make dinamic query


      connect(treeViewFaces, SIGNAL(expanded(QModelIndex)),  SLOT(slotFillGroup(QModelIndex)));
      connect(treeViewFaces, SIGNAL(collapsed(QModelIndex)), SLOT(slotClearGroup(QModelIndex)));
      connect(treeViewFaces, SIGNAL(clicked(QModelIndex)),   SLOT(slotFillPartner(QModelIndex)));

//    connect(treeViewCountry, SIGNAL(clicked(QModelIndex)),
//            this, SLOT(slotDataChanged(QModelIndex)));
//    connect(editDialogCountry, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
//    connect(editDialogCity, SIGNAL(saveDataChanged()), this, SLOT(slotInsertOrUpdateRecords()));
//    connect(newItem->ui->m_btnNext, SIGNAL(clicked()), editDialogCountry, SLOT(show()));
//    connect(newItem->ui->m_btnNext, SIGNAL(clicked()), SLOT(slotPushSelectRecordData()));
//    connect(newItem->ui->m_btnNext, SIGNAL(clicked()), newItem, SLOT(close()));

//     slotSelectRecordsFaces(actualRecords);
}

CCustomer::~CCustomer()
{
// drop #temporary table
    QString query ("DROP TABLE #GroupCustomerDiscounts"
                   "DROP TABLE #CustomerSubdiller");
    QSqlQuery temporary(currentDatabase());
    temporary.exec(query);

    if (IS_VALID_PTR(ui))  { delete ui; ui = nullptr; }
}

void CCustomer::fillFacesModel(const QModelIndex &index, QSqlQuery &stored)
{
   modelFaces->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    QString   it;
    int  ncols = stored.record().count();
    int  row(modelFaces->rowCount(index));

    if (stored.record().field(stored.record().indexOf("grp_parent")).isValid()){
    while(stored.next()){
        if (stored.value(stored.record().indexOf("grp_parent")).toInt() ==
            modelFaces->sibling(index.row(), 1, index).data().toInt()){
                 for (int i = 0; i != ncols; ++i){
                     it = stored.value(i).toString();
                     QStandardItem *item = new QStandardItem(it);
                     if (i == stored.record().indexOf("grp_name"))
                            item->setIcon(QIcon("data/picture/additionally/catalog.png"));

                     if (i == stored.record().indexOf("grp_name")){
                            modelFaces->itemFromIndex(index)->setChild(row, 0, item);
                     }
                     else if (i == stored.record().indexOf("grp_code")){
                            modelFaces->itemFromIndex(index)->setChild(row, 1, item);
                     }
                     else if (i == stored.record().indexOf("grp_parent")){
                            modelFaces->itemFromIndex(index)->setChild(row, 2, item);
                     }
                     else if (i == stored.record().indexOf("grp_actual")){
                            modelFaces->itemFromIndex(index)->setChild(row, 3, item);
                     }

                     if (item)
                         item->setChild(item->rowCount(), 0, new QStandardItem("Загрузка..."));
                 }
                 ++row;
        }
    }
    } else {
        while(stored.next())  {
            for (int i = 0; i != ncols; ++i){
                it = stored.value(i).toString();
                QStandardItem *item = new QStandardItem(it);

                if (i == stored.record().indexOf("par_name_first"))
                    item->setIcon(QIcon("data/picture/additionally/customer.png"));

                if (i == stored.record().indexOf("par_name_first")){
                    modelFaces->itemFromIndex(index)->setChild(row, 0, item);
                }
                else if (i == stored.record().indexOf("par_code")){
                    modelFaces->itemFromIndex(index)->setChild(row, 1, item);
                }
                else if (i == stored.record().indexOf("par_actual")){
                    modelFaces->itemFromIndex(index)->setChild(row, 2, item);
                }
            }
        row++;
        }
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CCustomer::fillPartnerModel(QSqlQuery &stored)
{
    modelPartner->clear();
    modelPartner->insertColumns(0, PARTNER_MODEL_COLUMN_COUNT);
    modelPartner->insertRows(stored.numRowsAffected(), 0);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    int  ncols = stored.record().count();
    int  row(0);

    while(stored.next()){
        for (int i = 0; i != ncols; ++i){
            if (i == stored.record().indexOf("par_name_first")){
                modelPartner->setItem(row, 0, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("null")){ // ?
                modelPartner->setItem(row, 1, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("null")){ // ?
                modelPartner->setItem(row, 2, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("cit_name_first")){
                modelPartner->setItem(row, 3, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_Web")){
                modelPartner->setItem(row, 4, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_Director")){
                modelPartner->setItem(row, 5, new QStandardItem(stored.value(i).toString()));
            }
            else if (i == stored.record().indexOf("par_comment")){
                textEditPartnerComment->setText(stored.value(i).toString());
            }
        }
        ++row;
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void CCustomer::columnHidden(const QVector<int> &vector)
{
   QVector<int>::const_iterator it = vector.begin();
   while(it != vector.end()){
       for (int i = 0; i != modelFaces->columnCount(); ++i){
           if (i == *it) continue;
           else
               treeViewFaces->setColumnHidden(i, true);
       }
     ++it;
   }
}

void CCustomer::slotFillGroup(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    if (modelFaces->hasChildren(index)){
        modelFaces->removeRows(0, modelFaces->rowCount(index), index);
    }

    list.append((int)GROUP_TYPE_CUSTOMER);
    list.append((int)actualRecords);
    list.append((int)SKIP); // parameter skip
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadAllReferenceGroups", storageHashTable(list));

    fillFacesModel(index, stored);

    if (root->index() != index){
        if(list.size() > 0)
           list.clear();

        list.append((int)actualRecords);
        list.append((int)index.sibling(index.row(), 1).data(Qt::DisplayRole).toInt());
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        list.append(QVariant::Invalid);
        stored.setForwardOnly(true);
        stored = execStored(currentDatabase(), "ReadAllCustomers", storageHashTable(list));

    fillFacesModel(index, stored);
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CCustomer::slotFillPartner(const QModelIndex &index)
{
    QList<QVariant> list;
    QSqlQuery       stored;

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(QPixmap("data/picture/additionally/wait.png")));
#endif

    list.append((int)index.sibling(index.row(), 1).data().toInt());
    stored.setForwardOnly(true);
    stored = execStored(currentDatabase(), "ReadOneCustomer", storageHashTable(list));

    fillPartnerModel(stored);

    modelPartner->setHeaderData(0, Qt::Horizontal, "Контрагенты");
    modelPartner->setHeaderData(1, Qt::Horizontal, "Тип");
    modelPartner->setHeaderData(2, Qt::Horizontal, "Статус");
    modelPartner->setHeaderData(3, Qt::Horizontal, "Город");
    modelPartner->setHeaderData(4, Qt::Horizontal, "Web");
    modelPartner->setHeaderData(5, Qt::Horizontal, "Руководитель");

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    stored.finish();
}

void CCustomer::slotClearGroup(const QModelIndex &index)
{
    if (index == root->index()){
        if (modelFaces->hasChildren(index)){
            modelFaces->removeRows(1, modelFaces->rowCount(index), index);
        }
    root->setChild(modelFaces->rowCount(root->index()), new QStandardItem("Загрузка..."));
    } else
    if (modelFaces->hasChildren(index)){
            modelFaces->removeRows(1, modelFaces->rowCount(index), index);
    }
    modelFaces->itemFromIndex(index)->setChild(modelFaces->rowCount(index), new QStandardItem("Загрузка..."));
}

CCustomerTreeView::CCustomerTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void CCustomerTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void CCustomerTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void CCustomerTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    CCustomerTreeView *source = qobject_cast<CCustomerTreeView*>(event->source()); // target QLineEdit

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCustomerTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    CCustomerTreeView *source = qobject_cast<CCustomerTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCustomerTreeView::dropEvent(QDropEvent *event)
{
    QTreeView *source = qobject_cast<QTreeView*>(event->source());

    if (source && source != this){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CCustomerTreeView::draging()
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

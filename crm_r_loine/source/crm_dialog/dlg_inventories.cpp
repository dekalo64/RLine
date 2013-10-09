#include "ui_dlg_inventories.h"

#include "source/crm_dialog/dlg_inventories.h"
#include "source/crm_core/core_dictionarycore.h"

InventoriesDialog::InventoriesDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::InventoriesDialog)
  , menu(new QMenu(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    menu->addAction(new QAction(QObject::tr("Новый документ"), this));
    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addAction(new QAction(QObject::tr("Удалить документ"), this));

    QPixmap pixmapChoose("data/picture/additionally/choose.png");

    ui->buttonChoose->setIcon(QIcon(pixmapChoose));
    ui->buttonChoose->setText("\t");
    ui->buttonChoose->setStyleSheet("QToolButton {"
                                                  "border: 1px solid #515151;"
                                                  "border-radius: 2px;"
                                                  "background-color: #68a44a;"
                                    "}"
                                    "QToolButton::menu-button {"
                                                  "border: 1px solid #515151;"
                                                  "border-top-right-radius: 2px;"
                                                  "border-bottom-right-radius: 2px;"
                                    "}"
                                    " QToolButton::menu-arrow {"
                                                  "image: url(data/picture/additionally/toolbutton-handle.png);"
                                    "}");
    ui->buttonChoose->setMenu(menu);

    listWidgetAccountingTransaction = new ListWidgetAccountingTransaction(this);
    ui->hLayoutAccountingTransaction->addWidget(listWidgetAccountingTransaction);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
}

InventoriesDialog::~InventoriesDialog()
{
    if (IS_VALID_PTR(listWidgetAccountingTransaction)) { delete listWidgetAccountingTransaction; listWidgetAccountingTransaction = 0; }
    if (IS_VALID_PTR(ui))                              { delete ui; ui = 0; }
}

void InventoriesDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
}

void InventoriesDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

ListWidgetAccountingTransaction::ListWidgetAccountingTransaction(QWidget *parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
}

void ListWidgetAccountingTransaction::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QListWidget::mousePressEvent(event);
}

void ListWidgetAccountingTransaction::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QListWidget::mouseMoveEvent(event);
}

void ListWidgetAccountingTransaction::dragEnterEvent(QDragEnterEvent *event)
{
    // TreeView *source = qobject_cast<TreeView*>(event->source());
    // to do

//    if (source && source != this){
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }
}

void ListWidgetAccountingTransaction::dragMoveEvent(QDragMoveEvent *event)
{
    // TreeView *source = qobject_cast<TreeView*>(event->source());
    // to do

//    if (source && source != this){
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }
}

void ListWidgetAccountingTransaction::dropEvent(QDropEvent *event)
{
    // TreeView *source = qobject_cast<TreeView*>(event->source());
    // to do

//    QListWidgetItem *item = new QListWidgetItem(this);

//    if (source && source != this){
//        item->setText(event->mimeData()->text());
//        item->setIcon(QIcon("data/picture/additionally/accountingtransaction.png"));
//        addItem(item);

//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }

//    delete item;
}

void ListWidgetAccountingTransaction::draging()
{
    QListWidgetItem *item = currentItem();

    if (item){
         QMimeData *mimeData = new QMimeData;
                    mimeData->setText(item->text());

         QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(QPixmap("data/picture/additionally/dragdrop.png"));
    }
}

#include "source/dialog/inventoriesdialog.h"
#include "ui_inventoriesdialog.h"
#include "source/core/dictionarycore.h"

InventoriesDialog::InventoriesDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::InventoriesDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    QPixmap pixmapAdd("data/picture/toolbar/new.png");

    ui->buttonAdd->setIcon(QIcon(pixmapAdd));
    ui->buttonAdd->setCursor(Qt::PointingHandCursor);

    QPixmap pixmapEdit("data/picture/toolbar/edit.png");

    ui->buttonEdit->setIcon(QIcon(pixmapEdit));
    ui->buttonEdit->setCursor(Qt::PointingHandCursor);

    QPixmap pixmapDelete("data/picture/toolbar/delete.png");

    ui->buttonDelete->setIcon(QIcon(pixmapDelete));
    ui->buttonDelete->setCursor(Qt::PointingHandCursor);

    ui->hLayoutControlButton->setAlignment(ui->buttonDelete, Qt::AlignLeft);

    listWidgetAccountingTransaction = new ListWidgetAccountingTransaction(this);
    ui->hLayoutAccountingTransaction->addWidget(listWidgetAccountingTransaction);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->lineEditInventories, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->checkBoxActual, SIGNAL(clicked()), this, SLOT(slotCurrentChanged()));

    updateActions();
}

InventoriesDialog::~InventoriesDialog()
{
    if (IS_VALID_PTR(listWidgetAccountingTransaction)) { delete listWidgetAccountingTransaction; listWidgetAccountingTransaction = 0; }
    if (IS_VALID_PTR(ui))                              { delete ui; ui = 0; }
}

void InventoriesDialog::closeEvent(QCloseEvent *)
{
    DictionaryCore::clearEditDialog(this);
}

void InventoriesDialog::showEvent(QShowEvent *)
{
    updateActions();
}


void InventoriesDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

void InventoriesDialog::updateActions()
{
    bool m_enableBtnConnect = (!ui->lineEditInventories->text().isEmpty());
    ui->buttonSave->setEnabled(m_enableBtnConnect);
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

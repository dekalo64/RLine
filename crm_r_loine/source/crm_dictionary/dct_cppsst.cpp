#include "ui_dlg_cppsst.h"
#include "ui_dct_cppsst.h"

#include "source/crm_dictionary/dct_cppsst.h"
#include "source/crm_additionally/adl_communicate.h"

CCppsst::CCppsst(QWidget *parent) :
    QWidget(parent)
  , CDictionaryCore()
  , ui   (new Ui::CCppsst)
  , cppsstDialog (new CCppsstDialog(this))
{
    ui->setupUi(this);

    treeViewCppsst = new CTreeViewCppsst(this);
    treeViewCppsst->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeViewCppsst->header()->setVisible(true);
    ui->vLayoutTreeView->addWidget(treeViewCppsst);

    filter = new CFilter(this);
    filter->setObjectName("filter");
    filter->setPlaceholderText("Введите наименование");
    filter->installEventFilter(this);
    filter->setValidator(new QRegExpValidator(QRegExp(trUtf8("[а-яА-Яa-zA-Z0-9_]+")), this));
    ui->hLayoutSearchToItem->addWidget(filter);
}

CCppsst::~CCppsst()
{
    if (IS_VALID_PTR(cppsstDialog)) { delete cppsstDialog; cppsstDialog = nullptr; }
    if (IS_VALID_PTR(ui))           { delete ui;           ui           = nullptr; }
}

void CCppsst::columnHidden(QTreeView *view, QStandardItemModel *model, const QVector<int> &vector)
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

void CCppsst::slotCreateEditDialog(const int &r)
{
    Q_UNUSED(r);
}

CTreeViewCppsst::CTreeViewCppsst(QWidget *parent) :
    QTreeView(parent)
{
}

void CTreeViewCppsst::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void CTreeViewCppsst::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void CTreeViewCppsst::dragEnterEvent(QDragEnterEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CTreeViewCppsst::dragMoveEvent(QDragMoveEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CTreeViewCppsst::dropEvent(QDropEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        source->setText(event->mimeData()->text());

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CTreeViewCppsst::draging()
{
    QModelIndex *item = new QModelIndex(currentIndex());

    if (item->isValid()){
         QMimeData *mimeData = new QMimeData;
                    mimeData->setText(item->data().toString());

         QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(QPixmap("data/picture/additionally/dragdrop.png"));
    }
}

#include "source/dictionary/dictionarytemplate.h"
#include "ui_dictionarytemplate.h"
#include "ui_dictionarydialog.h"
#include "source/additionally/communicate.h"

DictionaryTemplate::DictionaryTemplate(QWidget *parent) :
    QWidget(parent)
  , CDictionaryCore()
  , ui   (new Ui::DictionaryTemplate)
  , dictionaryDialog (new DictionaryDialog(this))
{
    ui->setupUi(this);

    treeView = new TreeView(this);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->header()->setVisible(true);
    ui->vLayoutTreeView->addWidget(treeView);

    lineEditSearchToItem = new LineEdit(this);
    lineEditSearchToItem->setPlaceholderText("Введите наименование");
    ui->hLayoutSearchToItem->addWidget(lineEditSearchToItem);
}

DictionaryTemplate::~DictionaryTemplate()
{
    if (IS_VALID_PTR(dictionaryDialog)) { delete dictionaryDialog; dictionaryDialog = nullptr; }
    if (IS_VALID_PTR(ui))               { delete ui;  ui = nullptr; }
}

QToolButton *DictionaryTemplate::getClearButton(void) const
{
    return lineEditSearchToItem->clearButton;
}

void DictionaryTemplate::slotCreateEditDialog(int r)
{
    if (currentDatabase().isOpen()) {
        switch (r) {
        case 0:
            dictionaryDialog->setWindowTitle(QString("Добавить"));
            m_rad = RecordActionDatabase::ardInsert;
            dictionaryDialog->ui->labelDateI->setText(QString(tr("Дата создания")));
            break;
        case 1:
            dictionaryDialog->setWindowTitle(QString("Править"));
            m_rad = RecordActionDatabase::ardUpdate;
            dictionaryDialog->ui->labelDateI->setText(QString(tr("Дата редактирования")));
            break;
        default:
            break;
        }
        if (!m_selectedItem && m_rad == RecordActionDatabase::ardUpdate){
            Communicate::showing(QString("Не удается править,\n запись не выбрана"), 3000);
            return;
        } else {
            dictionaryDialog->show();
        }
    emit pushSelectRecordData();
   }
}

void DictionaryTemplate::slotClearSearchToItem()
{
    lineEditSearchToItem->undo();
    lineEditSearchToItem->backspace();
}

LineEdit::LineEdit(QWidget *parent)
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

void LineEdit::resizeEvent(QResizeEvent *)
{
   QSize szClear = clearButton->sizeHint();
   QSize szSearch = searchButton->sizeHint();
   int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
   clearButton->move(rect().right() - frameWidth - szClear.width(),
                     (rect().bottom() + 1 - szClear.height()) / 2);
   searchButton->move(rect().left() + 4,
                     (rect().bottom() + 1 - szSearch.height()) / 2);
}

void LineEdit::slotUpdateCloseButton(const QString& text)
{
    clearButton->setVisible(!text.isEmpty());
}


TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
    setAcceptDrops(true);
}

void TreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
       startPosition = event->pos();
    }
    QTreeView::mousePressEvent(event);
}

void TreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        int distance = (event->pos() - startPosition).manhattanLength();
        if (distance >= QApplication::startDragDistance()){
            draging();
        }
    }
    QTreeView::mouseMoveEvent(event);
}

void TreeView::dragEnterEvent(QDragEnterEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeView::dropEvent(QDropEvent *event)
{
    QLineEdit *source = qobject_cast<QLineEdit*>(event->source());

    if (source){
        source->setText(event->mimeData()->text());

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void TreeView::draging()
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

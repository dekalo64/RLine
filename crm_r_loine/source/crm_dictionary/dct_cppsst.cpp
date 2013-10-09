#include "ui_dlg_cppsst.h"
#include "ui_dct_cppsst.h"

#include "source/crm_dictionary/dct_cppsst.h"
#include "source/crm_additionally/adl_communicate.h"

CCppsst::CCppsst(QWidget *parent) :
    QWidget(parent)
  , CDictionaryCore()
  , ui (new Ui::CCppsst)
{
    ui->setupUi(this);

    treeCppsst = new CTreeViewCppsst(this);
    treeCppsst->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeCppsst->header()->setVisible(true);
    ui->vLayoutTreeView->addWidget(treeCppsst);

// dialog
    cppsstDialog = new CCppsstDialog(this);

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

QMenu *CCppsst::getContextMenu() const
{
    return treeCppsst->menu;
}

void CCppsst::slotCreateEditDialog(const QString &action)
{
    Q_UNUSED(action);
}

CTreeViewCppsst::CTreeViewCppsst(QWidget *parent) :
    QTreeView(parent)
  , menu(new QMenu(parent))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Копировать"), this));
    menu->addAction(new QAction(QObject::tr("Удалить"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QObject::tr("Обновить"), this));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(slotCustomContextMenuRequested(const QPoint &)));
}

CTreeViewCppsst::~CTreeViewCppsst()
{
    if IS_VALID_PTR(menu) { menu = nullptr; }
}

void CTreeViewCppsst::slotCustomContextMenuRequested(const QPoint &pos)
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
            disable.append(5);
    } else {
            disable.append(0);
            disable.append(2);
            disable.append(3);
    }
    for (auto i = disable.begin(); i != disable.end(); ++i){
        if (disable.contains(*i)){
            menu->actions().at(*i)->setEnabled(false);
        }
    }
    menu->exec(viewport()->mapToGlobal(pos));
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

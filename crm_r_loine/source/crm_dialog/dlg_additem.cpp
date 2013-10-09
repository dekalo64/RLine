#include "ui_dlg_additem.h"

#include "source/crm_dialog/dlg_additem.h"

CAddItem::CAddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddItem)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    setWindowTitle("Добавить");
    ui->radioAddCatalog->setIcon (QIcon("data/picture/additionally/catalog.ico"));
    ui->radioAddDocument->setIcon(QIcon("data/picture/additionally/document.ico"));

    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(close()));
}

CAddItem::~CAddItem()
{
    delete ui;
}

void CAddItem::showEvent(QShowEvent *)
{
    ui->radioAddCatalog->setChecked(true);
}


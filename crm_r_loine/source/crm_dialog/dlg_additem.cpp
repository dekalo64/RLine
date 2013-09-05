#include "ui_dlg_additem.h"

#include "source/crm_dialog/dlg_additem.h"

AddItem::AddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItem)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    setWindowTitle("Добавить");
    ui->radioNewCatalog->setIcon(QIcon("data/picture/additionally/catalog.ico"));
    ui->radioNewSubCatalog->setIcon(QIcon("data/picture/additionally/sub-catalog.ico"));
    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(close()));
}

AddItem::~AddItem()
{
    delete ui;
}

void AddItem::showEvent(QShowEvent *)
{
    ui->radioNewCatalog->setChecked(true);
}


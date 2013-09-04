#include "source/dialog/newitemdialog.h"
#include "ui_newitemdialog.h"

NewItemDialog::NewItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewItemDialog)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    setWindowTitle("Добавить");
    ui->radioNewCatalog->setIcon(QIcon("data/picture/additionally/catalog.ico"));
    ui->radioNewSubCatalog->setIcon(QIcon("data/picture/additionally/sub-catalog.ico"));
    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(close()));
}

NewItemDialog::~NewItemDialog()
{
    delete ui;
}

void NewItemDialog::showEvent(QShowEvent *)
{
    ui->radioNewCatalog->setChecked(true);
}


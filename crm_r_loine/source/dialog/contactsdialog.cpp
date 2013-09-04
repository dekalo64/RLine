#include "contactsdialog.h"
#include "ui_contactsdialog.h"

ContactsDialog::ContactsDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::ContactsDialog)
{ 
    this->setWindowFlags(Qt::Tool);
    ui->setupUi(this);
}

ContactsDialog::~ContactsDialog()
{
}

void ContactsDialog::closeEvent(QCloseEvent *)
{
}

void ContactsDialog::showEvent(QShowEvent *)
{
}

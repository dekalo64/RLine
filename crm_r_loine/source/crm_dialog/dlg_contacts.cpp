#include "ui_dlg_contacts.h"

#include "source/crm_dialog/dlg_contacts.h"

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

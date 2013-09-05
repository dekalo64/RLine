#include "ui_dct_contacts.h"

#include "source/crm_dictionary/dct_contacts.h"

Contacts::Contacts(QWidget *parent) :
    QWidget(parent)
  , ui  (new Ui::Contacts)
  , dialogEdit(new ContactsDialog(this) )
{
    ui->setupUi(this);

    connect(ui->buttonNew, SIGNAL(clicked()), dialogEdit, SLOT(show()));
}

Contacts::~Contacts()
{
    if (dialogEdit) { delete dialogEdit; dialogEdit = nullptr; }
    if (ui)         { delete ui;         ui         = nullptr; }
}

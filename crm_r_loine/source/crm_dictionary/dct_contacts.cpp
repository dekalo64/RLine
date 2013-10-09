#include "ui_dct_contacts.h"

#include "source/crm_dictionary/dct_contacts.h"

CContacts::CContacts(QWidget *parent) :
    QWidget(parent)
  , ui  (new Ui::Contacts)
  , dialogEdit(new ContactsDialog(this) )
{
    ui->setupUi(this);

    connect(ui->buttonNew, SIGNAL(clicked()), dialogEdit, SLOT(show()));
}

CContacts::~CContacts()
{
    if (dialogEdit) { delete dialogEdit; dialogEdit = nullptr; }
    if (ui)         { delete ui;         ui         = nullptr; }
}

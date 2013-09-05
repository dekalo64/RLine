#pragma once

#ifndef CONTACTS_H
#define CONTACTS_H

#include "source/crm_dialog/dlg_contacts.h"

#include <QtGui/QWidget>

namespace Ui {
class Contacts;
}

class Contacts : public QWidget
{
    Q_OBJECT
    
public:
    explicit Contacts(QWidget *parent = 0);
    ~Contacts();
    
private:
    Ui::Contacts   *ui;
    ContactsDialog *dialogEdit;
};

#endif // CONTACTS_H

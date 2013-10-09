#include "ui_dlg_customer_g.h"

#include "source/crm_dialog/dlg_customer_g.h"

CCustomer_gDialog::CCustomer_gDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCustomer_gDialog)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);
}

CCustomer_gDialog::~CCustomer_gDialog()
{
    delete ui;
}

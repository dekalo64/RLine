#include "dlg_discount.h"
#include "ui_dlg_discount.h"

CDiscountDialog::CDiscountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDiscountDialog)
{
    ui->setupUi(this);
}

CDiscountDialog::~CDiscountDialog()
{
    delete ui;
}

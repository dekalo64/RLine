#ifndef DLG_CUSTOMER_GDIALOG_H
#define DLG_CUSTOMER_GDIALOG_H

#include <QDialog>

namespace Ui {
class CCustomer_gDialog;
}

class CCustomer_gDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CCustomer_gDialog(QWidget *parent = 0);
    ~CCustomer_gDialog();
    
public:
    Ui::CCustomer_gDialog *ui;
};

#endif // DLG_CUSTOMER_GDIALOG_H

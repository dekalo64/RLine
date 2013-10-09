#ifndef DLG_DISCOUNTDIALOG_H
#define DLG_DISCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class CDiscountDialog;
}

class CDiscountDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDiscountDialog(QWidget *parent = 0);
    ~CDiscountDialog();
    
private:
    Ui::CDiscountDialog *ui;
};

#endif // DLG_DISCOUNTDIALOG_H

#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>

namespace Ui {
class CCustomerDialog;
}

class  CCustomerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CCustomerDialog(QWidget *parent = 0);
    ~CCustomerDialog();

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

public:
    Ui::CCustomerDialog *ui;
    QMenu               *menu;

};

#endif // CUSTOMERDIALOG_H

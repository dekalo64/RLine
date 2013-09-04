#ifndef NEWITEM_H
#define NEWITEM_H

#include <QtGui/QDialog>

namespace Ui {
class NewItemDialog;
}

class NewItemDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewItemDialog(QWidget *parent = 0);
    virtual ~NewItemDialog();

protected:
    void showEvent(QShowEvent *);

public:
    Ui::NewItemDialog *ui;
};

#endif // NEWITEM_H

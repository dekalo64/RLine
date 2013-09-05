#ifndef ADDITEM_H
#define ADDITEM_H

#include <QtGui/QDialog>

namespace Ui {
class AddItem;
}

class AddItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddItem(QWidget *parent = 0);
    virtual ~AddItem();

protected:
    void showEvent(QShowEvent *);

public:
    Ui::AddItem *ui;
};

#endif // NEWITEM_H

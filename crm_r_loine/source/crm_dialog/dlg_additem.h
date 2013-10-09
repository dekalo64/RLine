#ifndef ADDITEM_H
#define ADDITEM_H

#include <QtGui/QDialog>

namespace Ui {
class CAddItem;
}

class CAddItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit CAddItem(QWidget *parent = 0);
    virtual ~CAddItem();

protected:
    void showEvent(QShowEvent *);

public:
    Ui::CAddItem *ui;
};

#endif // NEWITEM_H

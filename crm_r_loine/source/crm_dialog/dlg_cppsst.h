#ifndef CCPPSSTDIALOG_H
#define CCPPSSTDIALOG_H

#include "source/crm_core/core_logisticnamespace.h"

#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>

namespace Ui {
class CCppsstDialog;
}

class CCppsstDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CCppsstDialog(QWidget *parent = 0);
    virtual ~CCppsstDialog();

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

signals:
    void saveDataChanged();

private slots:
    void slotCurrentChanged() { updateActions(); }
    void slotSaveDataChanged();
    
private:
    void updateActions();
    bool enableSave;

public:
    Ui::CCppsstDialog *ui;
};

#endif // CCPPSSTDIALOG_H

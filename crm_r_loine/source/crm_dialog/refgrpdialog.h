#ifndef REFGRPDIALOG_H
#define REFGRPDIALOG_H

#include <QDialog>

namespace Ui {
class RefGrpDialog;
}

class RefGrpDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RefGrpDialog(QWidget *parent = 0);
    ~RefGrpDialog();

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
    
public:
    Ui::RefGrpDialog *ui;
};

#endif // REFGRPDIALOG_H

#ifndef REFGRPDIALOG_H
#define REFGRPDIALOG_H

#include <QtGui/QDialog>

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

signals:
    void saveDataChanged();

private slots:
    void slotSaveDataChanged();
    
public:
    Ui::RefGrpDialog *ui;
};

#endif // REFGRPDIALOG_H

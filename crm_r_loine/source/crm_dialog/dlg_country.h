#ifndef COUNTRYDIALOG_H
#define COUNTRYDIALOG_H

#include <QDialog>

namespace Ui {
class CountryDialog;
}

class CountryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CountryDialog(QWidget *parent = 0);
    ~CountryDialog();

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
    Ui::CountryDialog *ui;
};

#endif // COUNTRYDIALOG_H

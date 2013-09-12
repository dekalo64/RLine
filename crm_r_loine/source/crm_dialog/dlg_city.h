#ifndef CITYDIALOG_H
#define CITYDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class CityDialog;
}

class CityDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CityDialog(QWidget *parent = 0);
    ~CityDialog();

private:
    void updateActions();
    bool enableSave;

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

signals:
    void saveDataChanged();

private slots:
    void slotCurrentChanged() { updateActions(); }
    void slotSaveDataChanged();

public:
    Ui::CityDialog *ui;
};

#endif // CITYDIALOG_H

#ifndef CITYDIALOG_H
#define CITYDIALOG_H

#include <QDialog>

namespace Ui {
class CityDialog;
}

class CityDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CityDialog(QWidget *parent = 0);
    ~CityDialog();

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
    Ui::CityDialog *ui;
};

#endif // CITYDIALOG_H

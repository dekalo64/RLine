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

    void fillFormSelectedRecord(const QList<QString> &param, const Action &act);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void saveDataChanged(const QList<QString> &param);

private slots:
    void slotSaveDataChanged();
    void slotEnabledComboBox(const bool &enabled);

private:
    Ui::CCppsstDialog *ui;
};

#endif // CCPPSSTDIALOG_H

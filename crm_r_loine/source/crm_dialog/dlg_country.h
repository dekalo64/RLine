#ifndef CCOUNTRYDIALOG_H
#define CCOUNTRYDIALOG_H

#include "source/crm_core/core_logisticnamespace.h"

#include <QtGui/QDialog>

namespace Ui {
class CCountryDialog;
}

class CCountryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CCountryDialog(QWidget *parent = 0);
    virtual ~CCountryDialog();

    void fillFormSelectedRecord(const QList<QString> &param, const Action &act);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void saveDataChanged(const QList<QString> &param);

private slots:
    void slotSaveDataChanged();

private:
    Ui::CCountryDialog *ui;
};

#endif // CCOUNTRYDIALOG_H

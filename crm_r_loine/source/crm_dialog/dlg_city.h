#ifndef CCITYDIALOG_H
#define CCITYDIALOG_H

#include "source/crm_core/core_logisticnamespace.h"

#include <QtGui/QDialog>

namespace Ui {
class CCityDialog;
}

class CCityDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CCityDialog(QWidget *parent = 0);
    virtual ~CCityDialog();

    void fillFormSelectedRecord(const QList<QString> &param, const Action &act);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void saveDataChanged(const QList<QString> &param);

private slots:
    void slotSaveDataChanged();

private:
    Ui::CCityDialog *ui;
};

#endif // CCITYDIALOG_H

#ifndef DLG_SUPPKIER_GDIALOG_H
#define DLG_SUPPKIER_GDIALOG_H

#include "source/crm_core/core_logisticnamespace.h"

#include <QtGui/QDialog>

namespace Ui {
class CSupplier_gDialog;
}

class CSupplier_gDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSupplier_gDialog(QWidget *parent = 0);
    virtual ~CSupplier_gDialog();

    void fillFormSelectedRecord(const QList<QVariant> &param, const Action &act);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void saveDataChanged(const QList<QString> &param);

private slots:
    void slotSaveDataChanged();
    
private:
    Ui::CSupplier_gDialog *ui;
};

#endif // DLG_SUPPKIER_GDIALOG_H

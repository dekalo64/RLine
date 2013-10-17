#pragma once

#ifndef SUPPLIERDIALOG_H
#define SUPPLIERDIALOG_H

#include "source/crm_core/core_logisticnamespace.h"

#include <QtGui>

namespace Ui {
class CSupplierDialog;
}

#define PRODUCER_MODEL_COLUMN_COUNT 2

class  CSupplierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSupplierDialog(QWidget *parent = 0);
    virtual ~CSupplierDialog();

    void fillFormSelectedRecord(const QList<QVariant> &param, const Action &act);

protected:
    void closeEvent(QCloseEvent *);

signals:
    void saveDataChanged(const QList<QString> &param);

private slots:
    void slotOpenDictionary();
    void slotInsertProducer();
    void slotDeleteProducer();
    void slotRefreshProducer();

    void slotSaveDataChanged();
    void slotTabCurrentChanged(const int &tab);

private:
    Ui::CSupplierDialog *ui;
    QStandardItemModel  *modelProducer;
    QItemSelectionModel *modelSelectionProducer;
    QMenu               *menu;

    bool                 removable;

};

#endif // SUPPLIERDIALOG_H

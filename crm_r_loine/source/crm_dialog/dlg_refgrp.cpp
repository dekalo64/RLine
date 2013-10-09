#include "ui_dlg_refgrp.h"

#include "source/crm_dialog/dlg_refgrp.h"
#include "source/crm_core/core_dictionarycore.h"

RefGrpDialog::RefGrpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefGrpDialog)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
}

RefGrpDialog::~RefGrpDialog()
{
    delete ui;
}

void RefGrpDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
}

void RefGrpDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

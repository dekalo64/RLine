#include "ui_dlg_cppsst.h"

#include "source/crm_dialog/dlg_cppsst.h"
#include "source/crm_core/core_dictionarycore.h"

CCppsstDialog::CCppsstDialog(QWidget *parent) :
      QDialog(parent)
    , enableSave(false)
    , ui(new Ui::CCppsstDialog)
{
    setWindowFlags(Qt::Drawer);
    setModal(true);

    ui->setupUi(this);

    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->lineEditName,  SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->comboBoxIcon,   SIGNAL(activated(int)), this, SLOT(slotCurrentChanged()));
    connect(ui->checkBoxActual, SIGNAL(clicked()), this, SLOT(slotCurrentChanged()));
    connect(ui->buttonSave,   SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));

    updateActions();
}

CCppsstDialog::~CCppsstDialog()
{
    delete ui;
}

void CCppsstDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
    enableSave = false;
}

void CCppsstDialog::showEvent(QShowEvent *)
{
    updateActions();
}

void CCppsstDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

void CCppsstDialog::updateActions()
{
    ui->buttonSave->setEnabled(enableSave);
    enableSave = ((!ui->lineEditName->text().isEmpty()));
}

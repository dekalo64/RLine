#include "source/dialog/refgrpdialog.h"
#include "ui_refgrpdialog.h"
#include "source/core/dictionarycore.h"

RefGrpDialog::RefGrpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefGrpDialog)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->lineEditRefGroup, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->checkBoxActual, SIGNAL(clicked()), this, SLOT(slotCurrentChanged()));

    updateActions();
}

RefGrpDialog::~RefGrpDialog()
{
    delete ui;
}

void RefGrpDialog::closeEvent(QCloseEvent *)
{
    DictionaryCore::clearEditDialog(this);
}

void RefGrpDialog::showEvent(QShowEvent *)
{
    updateActions();
}


void RefGrpDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

void RefGrpDialog::updateActions()
{
    bool m_enableBtnConnect = (!ui->lineEditRefGroup->text().isEmpty());
    ui->buttonSave->setEnabled(m_enableBtnConnect);
}

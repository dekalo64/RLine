#include "ui_dlg_country.h"

#include "source/crm_dialog/dlg_country.h"
#include "source/crm_core/core_dictionarycore.h"

CountryDialog::CountryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountryDialog)
{
    setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->lineEditName, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->lineEditNameEng, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->lineEditCityCode, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->checkBoxActual, SIGNAL(clicked()), this, SLOT(slotCurrentChanged()));

    updateActions();
}

CountryDialog::~CountryDialog()
{
    delete ui;
}

void CountryDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
}

void CountryDialog::showEvent(QShowEvent *)
{
    updateActions();
}

void CountryDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

void CountryDialog::updateActions()
{
    bool m_enableBtnConnect = (!ui->lineEditName->text().isEmpty() && !ui->lineEditNameEng->text().isEmpty() &&
                              !ui->lineEditCityCode->text().isEmpty());
    ui->buttonSave->setEnabled(m_enableBtnConnect);
}


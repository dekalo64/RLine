#include "source/dialog/citydialog.h"
#include "ui_citydialog.h"
#include "source/core/dictionarycore.h"

CityDialog::CityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CityDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Drawer);

    ui->lineEditCountryCode->setReadOnly(true);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->lineEditName, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->lineEditNameEng, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->lineEditCountry, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->lineEditCityCode, SIGNAL(textEdited(QString)), this, SLOT(slotCurrentChanged()));
    connect(ui->checkBoxActual, SIGNAL(clicked()), this, SLOT(slotCurrentChanged()));

    updateActions();
}

CityDialog::~CityDialog()
{
    delete ui;
}

void CityDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
}

void CityDialog::showEvent(QShowEvent *)
{
    updateActions();
}

void CityDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

void CityDialog::updateActions()
{
    bool enableBtnConnect = (!ui->lineEditName->text().isEmpty()   && !ui->lineEditNameEng->text().isEmpty() &&
                             !ui->lineEditCountry->text().isEmpty() && !ui->lineEditCityCode->text().isEmpty());
    ui->buttonSave->setEnabled(enableBtnConnect);
}


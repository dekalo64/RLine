#include "ui_dlg_country.h"

#include "source/crm_dialog/dlg_country.h"
#include "source/crm_core/core_dictionarycore.h"

CCountryDialog::CCountryDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::CCountryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    setModal(true);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
}

CCountryDialog::~CCountryDialog()
{
    delete ui;
}

void CCountryDialog::fillFormSelectedRecord(const QList<QString> &param, const Action &act)
{
    if (act == Action::Add){
        ui->labelUserD->setText(param.at(0));
        ui->labelDateD->setText(param.at(1));
    } else if (act == Action::Edit){
        setWindowTitle(windowTitle() + QString(" - [%1]").arg(param.at(0)));

        ui->lineEditName->setText      (param.at(0));
        ui->lineEditNameEng->setText   (param.at(1));
        ui->lineEditCityCode->setText  (param.at(2));
        ui->checkBoxActual->setChecked (QVariant(param.at(3)).toBool());
        ui->labelUserD->setText        (param.at(4));
        ui->labelDateD->setText        (param.at(5));
    }
}

void CCountryDialog::closeEvent(QCloseEvent *)
{
    setWindowTitle("Страна");
    CDictionaryCore::clearEditDialog(this);
}

void CCountryDialog::slotSaveDataChanged()
{
    QList<QString> param;

    param.append(ui->lineEditName->text());
    param.append(ui->lineEditNameEng->text());
    param.append(ui->lineEditCityCode->text());
    param.append(QString("%1").arg(ui->checkBoxActual->isChecked()));

    emit saveDataChanged(param);
}

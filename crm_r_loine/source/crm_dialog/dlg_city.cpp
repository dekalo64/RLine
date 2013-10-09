#include "ui_dlg_city.h"

#include "source/crm_dialog/dlg_city.h"
#include "source/crm_core/core_dictionarycore.h"

CCityDialog::CCityDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::CCityDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    setModal(true);

    connect(ui->buttonSave,   SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
}

CCityDialog::~CCityDialog()
{
    delete ui;
}

void CCityDialog::fillFormSelectedRecord(const QList<QString> &param, const Action &act)
{
    if (act == Action::Add){
        ui->lineEditCountry->setText     (param.at(0));
        ui->lineEditCountryCode->setText (param.at(1));
        ui->labelUserD->setText          (param.at(2));
        ui->labelDateD->setText          (param.at(3));
    } else if (act == Action::Edit){
        setWindowTitle(windowTitle() + QString(" - [%1]").arg(param.at(1)));

        ui->lineEditCountry->setText     (param.at(0));
        ui->lineEditName->setText        (param.at(1));
        ui->lineEditNameEng->setText     (param.at(2));
        ui->lineEditCountryCode->setText (param.at(3));
        ui->lineEditCityCode->setText    (param.at(4));
        ui->checkBoxActual->setChecked   (QVariant(param.at(5)).toBool());
        ui->labelUserD->setText          (param.at(6));
        ui->labelDateD->setText          (param.at(7));
    }
}

void CCityDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
    setWindowTitle("Город/Регион");
}

void CCityDialog::slotSaveDataChanged()
{
    setWindowTitle("Город/Регион");

    QList<QString> param;

    param.append(ui->lineEditName->text());
    param.append(ui->lineEditNameEng->text());
    param.append(ui->lineEditCityCode->text());
    param.append(QString("%1").arg(ui->checkBoxActual->isChecked()));

    emit saveDataChanged(param);
}

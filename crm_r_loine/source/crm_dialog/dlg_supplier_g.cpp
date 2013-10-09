#include "ui_dlg_supplier_g.h"

#include "source/crm_dialog/dlg_supplier_g.h"
#include "source/crm_core/core_dictionarycore.h"

CSupplier_gDialog::CSupplier_gDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSupplier_gDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
}

CSupplier_gDialog::~CSupplier_gDialog()
{
    delete ui;
}

void CSupplier_gDialog::fillFormSelectedRecord(const QList<QVariant> &param, const Action &act)
{
    if (act == Action::Add){

        ui->labelUserD->setText(QVariant(param.at(0)).toString());
        ui->labelDateD->setText(QVariant(param.at(1)).toString());

    } else if (act == Action::Edit){
        setWindowTitle(windowTitle() + QString(" - [%1]").arg(QVariant(param.at(0)).toString()));

        ui->lineEditName->setText      (QVariant(param.at(0)).toString());
        ui->checkBoxActual->setChecked (QVariant(param.at(1)).toBool());
        ui->labelUserD->setText        (QVariant(param.at(2)).toString());
        ui->labelDateD->setText        (QVariant(param.at(3)).toString());
    }
}

void CSupplier_gDialog::closeEvent(QCloseEvent *)
{
    setWindowTitle("Папка");
    CDictionaryCore::clearEditDialog(this);
}

void CSupplier_gDialog::slotSaveDataChanged()
{
    QList<QString> param;

    param.append(ui->lineEditName->text());
    param.append(QString("%1").arg(ui->checkBoxActual->isChecked()));

    emit saveDataChanged(param);
}

#include "ui_dlg_cppsst.h"

#include "source/crm_dialog/dlg_cppsst.h"
#include "source/crm_core/core_dictionarycore.h"

CCppsstDialog::CCppsstDialog(QWidget *parent) :
      QDialog(parent)
    , ui(new Ui::CCppsstDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Drawer);
    setModal(true);

    ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/red.png"),    "Высокий");
    ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/yellow.png"), "Средний");
    ui->comboBoxIcon->addItem(QIcon("data/picture/additionally/green.png"),  "Низкий");

    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->buttonSave,   SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
}

CCppsstDialog::~CCppsstDialog()
{
    delete ui;
}

void CCppsstDialog::fillFormSelectedRecord(const QList<QString> &param, const Action &act)
{
    if (act == Action::Add){

        ui->comboBoxIcon->setCurrentIndex(QVariant(param.at(0)).toInt());
        ui->labelUserD->setText(param.at(1));
        ui->labelDateD->setText(param.at(2));

    } else if (act == Action::Edit){

        setWindowTitle(windowTitle() + QString(" - [%1]").arg(param.at(0)));

        ui->lineEditName->setText         (param.at(0));
        ui->comboBoxIcon->setCurrentIndex (QVariant(param.at(1)).toInt());
        ui->checkBoxActual->setChecked    (QVariant(param.at(2)).toBool());
        ui->labelUserD->setText           (param.at(3));
        ui->labelDateD->setText           (param.at(4));

    }
}

void CCppsstDialog::closeEvent(QCloseEvent *)
{
    windowTitle().clear();
    CDictionaryCore::clearEditDialog(this);
}

void CCppsstDialog::slotSaveDataChanged()
{
    QList<QString> param;

    param.append(ui->lineEditName->text());
    param.append(QVariant(ui->comboBoxIcon->currentIndex()).toString());
    param.append(QVariant(ui->checkBoxActual->isChecked()).toString());

    emit saveDataChanged(param);
}

void CCppsstDialog::slotEnabledComboBox(const bool &enabled)
{
    ui->comboBoxIcon->setEnabled (enabled);
    ui->comboBoxIcon->setEditable(enabled);
}

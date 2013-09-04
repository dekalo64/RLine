#include "source/dialog/dictionarydialog.h"
#include "ui_dictionarydialog.h"
#include "source/core/dictionarycore.h"

DictionaryDialog::DictionaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictionaryDialog)
{
    this->setWindowFlags(Qt::Drawer);
    ui->setupUi(this);

    connect(ui->buttonCancel, SIGNAL(clicked()),            this, SLOT(close()));
    connect(ui->lineEditItem,  SIGNAL(textEdited(QString)),  this, SLOT(slotCurrentChanged()));
    connect(ui->comboBoxIcon,   SIGNAL(activated(int)),       this, SLOT(slotCurrentChanged()));
    connect(ui->checkBoxActual, SIGNAL(clicked()),            this, SLOT(slotCurrentChanged()));
    connect(ui->buttonSave,   SIGNAL(clicked()),            this, SLOT(slotSaveDataChanged()));

    updateActions();
}

DictionaryDialog::~DictionaryDialog()
{
    delete ui;
}

void DictionaryDialog::closeEvent(QCloseEvent *)
{
    CDictionaryCore::clearEditDialog(this);
}

void DictionaryDialog::showEvent(QShowEvent *)
{
    updateActions();
}

void DictionaryDialog::slotSaveDataChanged()
{
    emit saveDataChanged();
}

void DictionaryDialog::updateActions()
{
    bool m_enableBtnConnect = ((!ui->lineEditItem->text().isEmpty()));
    ui->buttonSave->setEnabled(m_enableBtnConnect);
}

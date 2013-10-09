#include "ui_dlg_supplier.h"

#include "source/crm_dialog/dlg_supplier.h"
#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticmainwindow.h"

CSupplierDialog::CSupplierDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::CSupplierDialog)
  , menu(new QMenu(this))
{ 
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

// model
    modelProducer = new QStandardItemModel(this);
    modelProducer->insertColumns(0, PRODUCER_MODEL_COLUMN_COUNT);
    modelProducer->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименовние"));

    modelSelectionProducer = new QItemSelectionModel(modelProducer);

    ui->treeProducer->setModel(modelProducer);
    ui->treeProducer->setSelectionModel(modelSelectionProducer);

    QVector<int> storage;
                 storage.append(0);
    CDictionaryCore::columnHidden(ui->treeProducer, modelProducer, storage);
                 storage.clear();

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));
    menu->addAction(new QAction(QObject::tr("Удалить документ"), this));

    ui->comboLanguage->addItem(QIcon("data/picture/additionally/russian.png"), "Русский");
    ui->comboLanguage->addItem(QIcon("data/picture/additionally/germany.png"), "Немецкий");
    ui->comboLanguage->addItem(QIcon("data/picture/additionally/england.png"), "Английский");

    ui->comboCurrency->addItem(QIcon("data/picture/additionally/usa.png"),     "Доллар");
    ui->comboCurrency->addItem(QIcon("data/picture/additionally/europe.png"),  "Евро");
    ui->comboCurrency->addItem(QIcon("data/picture/additionally/ukraine.png"), "Гривна");

    QPixmap pixmapChoose("data/picture/additionally/choose.png");

    QList<QToolButton*> button;

    button.append(ui->buttonCity);
    button.append(ui->buttonProducer);

    for (QToolButton *currentButton : button){
        currentButton->setIcon(QIcon(pixmapChoose));
        currentButton->setText("\t");
        currentButton->setStyleSheet("QToolButton {"
                                     "border: 1px solid #515151;"
                                     "border-radius: 2px;"
                                     "background-color: #68a44a;"
                       "}"
                       "QToolButton::menu-button {"
                                     "border: 1px solid #515151;"
                                     "border-top-right-radius: 2px;"
                                     "border-bottom-right-radius: 2px;"
                       "}"
                       " QToolButton::menu-arrow {"
                                     "image: url(data/picture/additionally/toolbutton-handle.png);"
                       "}");
        currentButton->setMenu(menu);
    }

    connect(menu->actions().at(0), SIGNAL(triggered()), SLOT(slotOpenDictionary()));
    connect(menu->actions().at(1), SIGNAL(triggered()), SLOT(slotDeleteProducer()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(slotTabCurrentChanged(int)));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(slotSaveDataChanged()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(close()));

    slotTabCurrentChanged(0);
}

CSupplierDialog::~CSupplierDialog()
{
    if (IS_VALID_PTR(modelProducer))          { /*delete modelProducer;*/          modelProducer          = nullptr; }
    if (IS_VALID_PTR(modelSelectionProducer)) { /*delete modelSelectionProducer;*/ modelSelectionProducer = nullptr; }

    delete ui;
}

void CSupplierDialog::fillFormSelectedRecord(const QList<QVariant> &param, const Action &act)
{
    if (act == Action::Add){

        ui->editClientNumber->setText(QVariant(param.at(0)).toString());
        ui->editCity->setText(QString("%1, %2").arg(QVariant(param.at(1)).toString()).arg(QVariant(param.at(2)).toString()));
        ui->comboCurrency->setCurrentIndex(QVariant(param.at(3)).toInt());
        ui->editPayment->setText(QVariant(param.at(4)).toString());
        ui->comboLanguage->setCurrentIndex(QVariant(param.at(5)).toInt());
        ui->spinPrepayment->setValue(QVariant(param.at(6)).toFloat() * 100);
        ui->labelUserD->setText(QVariant(param.at(7)).toString());
        ui->labelDateD->setText(QVariant(param.at(8)).toString());

    } else if (act == Action::Edit){

        setWindowTitle(windowTitle() + QString(" - [%1]").arg(QVariant(param.at(0)).toString()));

        ui->editName->setText(QVariant(param.at(0)).toString());
        ui->editNameEng->setText(QVariant(param.at(1)).toString());
        ui->editClientNumber->setText(QVariant(param.at(2)).toString());
        ui->editCity->setText(QString("%1, %2").arg(QVariant(param.at(3)).toString()).arg(QVariant(param.at(4)).toString()));
        ui->editAddress->setText(QVariant(param.at(5)).toString());
        ui->editAttender->setText(QVariant(param.at(6)).toString());
        ui->editMobilePhone->setText(QVariant(param.at(7)).toString());
        ui->editWorkPhone->setText(QString("%1 (%2) %3").arg(QVariant(param.at(8)).toString()).arg(QVariant(param.at(9)).toString()).arg(QVariant(param.at(10)).toString()));
        ui->editFax->setText(QString("%1 (%2) %3").arg(QVariant(param.at(8)).toString()).arg(QVariant(param.at(9)).toString()).arg(QVariant(param.at(11)).toString()));
        ui->editPostIndex->setText(QVariant(param.at(12)).toString());
        ui->editDateOrg->setText(QVariant(param.at(13)).toDate().toString("dd.MM.yyyy"));
        ui->editEmail->setText(QVariant(param.at(14)).toString());
        ui->editWeb->setText(QVariant(param.at(16)).toString());
        ui->editDirector->setText(QVariant(param.at(17)).toString());
        ui->comboCurrency->setCurrentIndex(QVariant(param.at(18)).toUInt());
        ui->editPayment->setText(QVariant(param.at(19)).toString());
        ui->comboLanguage->setCurrentIndex(QVariant(param.at(20)).toUInt());
        ui->editZKPO->setText(QVariant(param.at(21)).toString());

        QPalette palette = ui->editBalance->palette();
        if (QVariant(param.at(22)).toInt() == 0){
            palette.setColor(ui->editBalance->foregroundRole(), Qt::black);
            ui->editBalance->setPalette(palette);
            ui->editBalance->setText("нет");
        } else if (QVariant(param.at(22)).toInt() < 0){
            palette.setColor(ui->editBalance->foregroundRole(), Qt::red);
            ui->editBalance->setPalette(palette);
            ui->editBalance->setText(QVariant(param.at(22)).toString());
        } else if (QVariant(param.at(22)).toInt() > 0){
            palette.setColor(ui->editBalance->foregroundRole(), Qt::green);
            ui->editBalance->setPalette(palette);
            ui->editBalance->setText(QVariant(param.at(22)).toString());
        }

        ui->editINN->setText(QVariant(param.at(23)).toString());
        ui->editNDS->setText(QVariant(param.at(24)).toString());
        ui->editMFO->setText(QVariant(param.at(25)).toString());
        ui->dateIssuance->setDate(QVariant(param.at(26)).toDate());
        ui->editAccount->setText(QVariant(param.at(27)).toString());
        ui->editInBank->setText(QVariant(param.at(28)).toString());
        ui->editIBAN->setText(QVariant(param.at(29)).toString());
        ui->editComment->setText(QVariant(param.at(30)).toString());
        ui->spinServices->setValue(QVariant(param.at(31)).toFloat() * 100);
        ui->spinGoods->setValue(QVariant(param.at(32)).toFloat() * 100);
        ui->spinDeliveryTime->setValue(QVariant(param.at(33)).toUInt());
        ui->spinPrepayment->setValue(QVariant(param.at(34)).toFloat() * 100);

        QHash<QString, QVariant> hash = QVariant(param.at(35)).toHash();
        short   rows(0);

        modelProducer->removeRows(0, modelProducer->rowCount(QModelIndex()), QModelIndex());

        QHash<QString, QVariant>::const_iterator i = hash.constBegin();
        for (; i != hash.constEnd(); ++i){
            modelProducer->setItem(rows, 0, new QStandardItem(i.key()));
            modelProducer->setItem(rows, 1, new QStandardItem(QIcon("data/picture/additionally/processor.png"), QVariant(i.value()).toString()));

            ++rows;
        }

        ui->checkActual->setChecked (QVariant(param.at(36)).toBool());
        ui->labelUserD->setText(QVariant(param.at(37)).toString());
        ui->labelDateD->setText (QVariant(param.at(38)).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
}

void CSupplierDialog::closeEvent(QCloseEvent *)
{
    setWindowTitle("Поставщик");
    CDictionaryCore::clearEditDialog(this);
}

void CSupplierDialog::slotSaveDataChanged()
{
    QList<QString> param;

    /*
      param.append(...);
    */

    emit saveDataChanged(param);
}

void CSupplierDialog::slotTabCurrentChanged(const int &tab)
{
    if (tab == 0){
        menu->actions().at(1)->setEnabled(false);
    } else if (tab == 2){
        menu->actions().at(1)->setEnabled(true);
    }
}

void CSupplierDialog::slotOpenDictionary()
{
    if (ui->tabWidget->currentIndex() == 0){
        CLogisticMainWindow::instance()->slotCountryCityDictionary();
    } else if (ui->tabWidget->currentIndex() == 2){
        CLogisticMainWindow::instance()->slotProdusersDictionary();
    }
}

void CSupplierDialog::slotInsertProducer()
{
    QMessageBox::information(0, "title", "ins");
}

void CSupplierDialog::slotDeleteProducer()
{
    QList<QVariant> list;
    QSqlQuery       stored;

    if (focusWidget()->objectName() == ui->treeProducer->objectName()){
        if (modelSelectionProducer->selection().isEmpty()){
            CCommunicate::showing(QString("Не удается выполнить, производитель не выбран"));
            return;
        }
    }

    QMessageBox answer;
                answer.setText(QString("Подтверждаете удаление?"));
                answer.setWindowTitle(QString("Удаление"));
                answer.setIcon(QMessageBox::Question);

    QPushButton *_delete = answer.addButton(QString("Удалить"), QMessageBox::ActionRole);
    QPushButton *cancel  = answer.addButton(QString("Отмена"),  QMessageBox::ActionRole);

    answer.exec();

    if (answer.clickedButton() == _delete){
        const unsigned code = modelSelectionProducer->currentIndex().sibling(modelSelectionProducer->currentIndex().row(), 0).data().toUInt();
        list.append(code);

        stored = CDictionaryCore::execStored(CDictionaryCore::currentDatabase(), "DeleteProducerGroup", CDictionaryCore::storageHashTable(list));
        stored.finish();

    } else if (answer.clickedButton() == cancel){
        answer.reject();
    }
}

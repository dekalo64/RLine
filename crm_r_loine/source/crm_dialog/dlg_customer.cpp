#include "ui_dlg_customer.h"

#include "source/crm_dialog/dlg_customer.h"

CCustomerDialog::CCustomerDialog(QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::CCustomerDialog)
  , menu(new QMenu(this))
{ 
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    menu->addAction(new QAction(QObject::tr("Открыть документ"), this));

    QPixmap pixmapChoose("data/picture/additionally/choose.png");

    QList<QToolButton*> button;

    button.append(ui->buttonContact);
    button.append(ui->buttonDirector);
    button.append(ui->buttonCity);

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
}

CCustomerDialog::~CCustomerDialog()
{
}

void CCustomerDialog::closeEvent(QCloseEvent *)
{
}

void CCustomerDialog::showEvent(QShowEvent *)
{
}

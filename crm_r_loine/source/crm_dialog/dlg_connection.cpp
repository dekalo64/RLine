#include "ui_dlg_connection.h"

#include "source/crm_dialog/dlg_connection.h"

ConnectionDialog::ConnectionDialog(QWidget *parent)
    :QDialog(parent)
  , ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    setModal(true);

    QPixmap pixmapLogin("data/picture/additionally/login.png");
    QToolButton *login  = new QToolButton(this);
                 login->setIcon(QIcon(pixmapLogin));
                 login->setIconSize(pixmapLogin.size());
                 login->setStyleSheet("QToolButton { border: none; padding: 0px; }");
                 login->setParent(ui->lineEditUserName);

    QPixmap pixmapPassword("data/picture/additionally/password.png");
    QToolButton *password = new QToolButton(this);
                 password->setIcon(QIcon(pixmapPassword));
                 password->setIconSize(pixmapPassword.size());
                 password->setStyleSheet("QToolButton { border: none; padding: 0px; }");
                 password->setParent(ui->lineEditPassword);

    int fwLogin = ui->lineEditUserName->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    ui->lineEditUserName->setStyleSheet(QString("QLineEdit { padding-left: %1px; } ").arg(login->sizeHint().width() + fwLogin + 1));

    int fwPassword = ui->lineEditPassword->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    ui->lineEditPassword->setStyleSheet(QString("QLineEdit { padding-left: %1px; } ").arg(password->sizeHint().width() + fwPassword + 1));

                 QSize szLogin = login->sizeHint();
                 QSize szPassword = password->sizeHint();

                 login->move(   ui->lineEditUserName->rect().left() + 4,
                                    (ui->lineEditUserName->rect().bottom() + 1 - szLogin.height()) / 3);
                 password->move(ui->lineEditPassword->rect().left() + 4,
                                    (ui->lineEditPassword->rect().bottom() + 1 - szPassword.height()) / 3);

    connect(ui->lineEditUserName, SIGNAL(textEdited(QString)), this, SLOT(currentChanged()));
    connect(ui->buttonDatabaseConnect, SIGNAL(clicked()), SLOT(slotDataBaseConnect()));
    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(slotExitApplication()));

    updateActions();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui; ui = 0;
}

void ConnectionDialog::updateActions()
{
    bool m_enableBtnConnect = ((!ui->lineEditUserName->text().isEmpty()));
    ui->buttonDatabaseConnect->setEnabled(m_enableBtnConnect);
}

QString ConnectionDialog::userName() const
{
    return ui->lineEditUserName->text();
}

QString ConnectionDialog::password() const
{
    return ui->lineEditPassword->text();
}

void ConnectionDialog::slotDataBaseConnect()
{
    emit sendUserInformation(userName(), password());
}

void ConnectionDialog::slotSuccessConnection()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");

    animation->setDuration(500);
    animation->setStartValue(1);
    animation->setEndValue  (0);

    connect(animation, SIGNAL(finished()), this, SLOT(close()));
    animation->start(QPropertyAnimation::DeleteWhenStopped);

    emit setVisibleComponent(true);
}

#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent)
    :QDialog(parent)
  , ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);

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

void ConnectionDialog::paintEvent(QPaintEvent* /* event */)
{
    updateMask();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(rect().adjusted(2, 1, -2, -1), 5, 5);
}

QRegion ConnectionDialog::getRegion() const
{
    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1, -1, -1, 0), 8, 8);
    return QRegion(path.toFillPolygon().toPolygon());
}

void ConnectionDialog::updateMask()
{
    QBitmap Bitmap(size());
    Bitmap.clear();
    QPainter Painter(&Bitmap);

    Painter.setClipRegion(getRegion());
    Painter.fillRect(rect(), Qt::color1);
    Painter.end();

    setMask(Bitmap);
}

#include "source/additionally/communicate.h"
#include "source/core/logisticapplication.h"
#include "source/core/logisticmainwindow.h"

static Communicate *theSolitaryCommunicate = nullptr;

QLabel *Communicate::showing(const QString &text, int timeout){
    hiding();
    theSolitaryCommunicate = new Communicate(text);
    if (timeout <= 0)
        timeout = 10000;
    theSolitaryCommunicate->window(timeout);
    return theSolitaryCommunicate;
}

void Communicate::hiding()
{
    if (!theSolitaryCommunicate) return;
    theSolitaryCommunicate->hide();
    delete theSolitaryCommunicate; theSolitaryCommunicate = nullptr;
}

Communicate::Communicate(const QString &text)
    : QLabel(LogisticApplication::instance()->mainWindow(), Qt::FramelessWindowHint | Qt::Window), m_timerId(-1)
{
    setCursor(QCursor(Qt::PointingHandCursor));

    m_pixmap = QPixmap("data/picture/communicate/communicate.png");

    m_lblTitle = new QLabel(this);
    m_lblTitle->setText(text);
    m_lblTitle->setCursor(QCursor(Qt::PointingHandCursor));
}

Communicate::~Communicate()
{
    theSolitaryCommunicate = nullptr;
    delete m_lblTitle; m_lblTitle = nullptr;
}

void Communicate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), m_pixmap);
}

void Communicate::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void Communicate::window(int msecs)
{
    setPixmap(m_pixmap);
    setMask(m_pixmap.mask());

    if (msecs > 0)
        m_timerId = startTimer(msecs);

    centerOnApplication();
    show();
}

void Communicate::centerOnApplication()
{
    move(LogisticApplication::instance()->mainWindow()->geometry().center().x() - rect().center().x(),
         LogisticApplication::instance()->mainWindow()->geometry().center().y() - rect().center().y());
}

void Communicate::mousePressEvent(QMouseEvent * /* e */)
{
    close();
}

void Communicate::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timerId) {
        killTimer(m_timerId);
        if (!underMouse())
            close();
        return;
    }
    QLabel::timerEvent(e);
}

#pragma once

#ifndef  COMMUNICATE_H
#define  COMMUNICATE_H

#include <QtGui>
#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

class Communicate : public QLabel
{
public:
    static QLabel *showing(const QString &text, int timeout = 0);
    static void    hiding();

private:
    explicit Communicate(const QString &text);
    virtual  ~Communicate();
    void window(int);
    void centerOnApplication();

protected:
    void paintEvent (QPaintEvent *);
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void timerEvent (QTimerEvent *e);

private:
    QLabel *m_lblTitle;

    QPixmap m_pixmap;
    int     m_timerId;
};

#endif // COMMUNICATE_H

#pragma once

#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/qevent.h>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>

class MdiWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MdiWindow(QWidget *parent = 0);
    virtual ~MdiWindow(){}

protected:
    void closeEvent(QCloseEvent *event);


};

#endif // MDIWINDOW_H

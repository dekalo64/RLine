#pragma once

#ifndef CMESSAGE_H
#define CMESSAGE_H

#include <QtGui>

class CMessage : public QMessageBox
{
public:
     explicit CMessage(QWidget *parent, const QString title, const QString text);
     virtual  ~CMessage();

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // CMESSAGE_H

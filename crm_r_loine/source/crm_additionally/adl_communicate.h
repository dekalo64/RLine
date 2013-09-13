#pragma once

#ifndef  COMMUNICATE_H
#define  COMMUNICATE_H

#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtGui>

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#else
#include <QtGui>
#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#endif

class CCommunicate : public QDialog
{
    Q_OBJECT

public:
    static QDialog *showing(const QString &text);
    static void     hiding();

private:
    explicit CCommunicate(const QString &text);
    virtual  ~CCommunicate();
    void window();
    void centerOnApplication();
};

#endif // COMMUNICATE_H

#pragma once

#ifndef DICTIONARYCORE_H
#define DICTIONARYCORE_H

#include <QtCore/QHash>
#include <QtCore/QModelIndex>
#include <QtGui/QWidget>
#include <QtGui/QToolBar>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#define IS_VALID_PTR(ptr) ((ptr != (void*)0))

QT_BEGIN_NAMESPACE
class QDialog;
QT_END_NAMESPACE

class CDictionaryCore
{
protected:
    virtual QSqlQuery execStored(const QSqlDatabase &database,
                         const QString &storedFunction,
                         const QHash<QString, QVariant> &hash = QHash<QString, QVariant>());
    QSqlDatabase currentDatabase(void) const;
    const QString currentUser(void);
    const QHash<QString, QVariant> storageHashTable(QList<QVariant> &list);

public:
    static void clearEditDialog(QDialog *editDialog);

};

class CFilter : public QLineEdit
{
    Q_OBJECT

public:
   CFilter(QWidget *parent = 0);
   virtual  ~CFilter();

protected:
    void resizeEvent(QResizeEvent *);

private:
    QToolButton *clearButton;
    QToolButton *searchButton;

private slots:
    void slotUpdateCloseButton(const QString &text);
    void slotClearSearchToItem (void);
};

#endif // DICTIONARYCORE_H

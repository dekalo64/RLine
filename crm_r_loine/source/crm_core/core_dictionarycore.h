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
class QTreeView;
class QStandardItemModel;
QT_END_NAMESPACE 

class CDictionaryCore
{
protected:
    const QString currentUser(void);

public:
    static QSqlQuery execStored(const QSqlDatabase &database,
                         const QString &storedFunction,
                         const QHash<QString, QVariant> &hash = QHash<QString, QVariant>());
    static const QSqlDatabase currentDatabase(void);
    static const QHash<QString, QVariant> storageHashTable(const QList<QVariant> &list = QList<QVariant>());
    static void clearEditDialog(QDialog *dialog);

    template <class T, class Q, template <class> class W>
              static void columnHidden (T *view, Q *model, W<int> &storage){
                      for (short i(0); i != model->columnCount(); ++i){
                          if (storage.contains(i))
                              view->setColumnHidden(i, true);
                      }
              }
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

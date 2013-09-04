#ifndef DICTIONARYEDIT_H
#define DICTIONARYEDIT_H

#include "source/core/logisticnamespace.h"

#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>

namespace Ui {
class DictionaryDialog;
}

class DictionaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DictionaryDialog(QWidget *parent = 0);
    virtual ~DictionaryDialog();

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

signals:
    void saveDataChanged();

private slots:
    void slotCurrentChanged() { updateActions(); }
    void slotSaveDataChanged();
    
private:
    void updateActions();

public:
    Ui::DictionaryDialog *ui;
};

#endif // DICTIONARYEDIT_H

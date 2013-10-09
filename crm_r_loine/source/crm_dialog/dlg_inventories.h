#ifndef INVENTORIESDIALOG_H
#define INVENTORIESDIALOG_H

#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QListWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>

namespace Ui {
class InventoriesDialog;
}

class ListWidgetAccountingTransaction;

class InventoriesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InventoriesDialog(QWidget *parent = 0);
    ~InventoriesDialog();

protected:
    void closeEvent(QCloseEvent *);

signals:
    void saveDataChanged();

private slots:
    void slotSaveDataChanged();

public:
    Ui::InventoriesDialog *ui;
    QMenu                 *menu;
    ListWidgetAccountingTransaction *listWidgetAccountingTransaction;
};

class ListWidgetAccountingTransaction : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidgetAccountingTransaction (QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void   draging();
    QPoint startPosition;
};

#endif // INVENTORIESDIALOG_H

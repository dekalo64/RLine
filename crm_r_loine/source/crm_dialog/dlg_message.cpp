#include "source/crm_dialog/dlg_message.h"

CMessage::CMessage(QWidget *parent, const QString title, const QString text):
    QMessageBox(parent)
{
    setWindowFlags(Qt::Drawer);
    setWindowTitle(title);
    setText(text);

    QPixmap pixmap("data/picture/additionally/alert.png");
    setIconPixmap(pixmap);

    QFont font(QFont("Arial", 8));
          font.setBold (true);
    setFont(font);

    setStyleSheet("QMessageBox {"
                            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                            "stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
                            "stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
                  "}"
                  "QPushButton {"
                            "border: 1px solid #515151;"
                            "border-radius: 2px;"
                            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                 "stop: 0 #FFFFFF, stop: 0.5 #FFFFFF,"
                            "stop: 0.51 #FFFFFF, stop: 1 #e3e3e3);"

                            "color: #515151;"
                            "min-width: 80px;"
                            "min-height: 25px;"
                            "font: bold;"
                            "font-family: Arial;"
                  "}"
                  "QPushButton:pressed {"
                            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                 "stop: 0 #e3e3e3, stop: 0.50 #FFFFFF,"
                                                 "stop: 0.51 #FFFFFF, stop: 1 #e3e3e3);"
                            "color: #515151;"
                  "}");

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));
}

CMessage::~CMessage()
{
}

void CMessage::closeEvent(QCloseEvent *event)
{
    event->accept();
}

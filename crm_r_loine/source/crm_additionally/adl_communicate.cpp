#include "source/crm_additionally/adl_communicate.h"
#include "source/crm_core/core_logisticmainwindow.h"

static CCommunicate *solitaryCommunicate = nullptr;

QDialog *CCommunicate::showing(const QString &text){
    solitaryCommunicate = new CCommunicate(text);
    solitaryCommunicate->window();

    return solitaryCommunicate;
}

void CCommunicate::hiding()
{
    if (!solitaryCommunicate)
        return;

    solitaryCommunicate->hide();

    delete solitaryCommunicate; solitaryCommunicate = nullptr;
}

CCommunicate::CCommunicate(const QString &message)
    : QDialog(0, Qt::Window | Qt::CustomizeWindowHint)
{
    setObjectName("CCommunicate");

    setModal(true);

    setMaximumSize(QSize(280, 130));
    setMinimumSize(QSize(280, 130));

    QLabel *title = new QLabel("Ошибка", this);

    QFont fontTitle(QFont("Arial", 11));
          fontTitle.setBold (true);
    title->setFont(fontTitle);

    QLabel *image = new QLabel(this);
            image->setPixmap(QPixmap("data/picture/additionally/alert.png"));
            image->setMaximumSize(QSize(32, 32));

    QLabel *text = new QLabel(message, this);

    QFont fontText(QFont("Arial", 8));
          fontText.setBold (true);
    text->setFont(fontText);
    text->setWordWrap(true);

    QPushButton *buttonSave = new QPushButton("OK", this);
    buttonSave->setStyleSheet("QPushButton {"
                                        "border: 1px solid #515151;"
                                        "border-radius: 2px;"
                                        "font: bold;"
                                        "font-family: Arial;"
                                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                             "stop: 0 #FFFFFF, stop: 0.5 #FFFFFF,"
                                                             "stop: 0.51 #FFFFFF, stop: 1 #e3e3e3);"

                                        "color: #515151;"
                                        "min-width: 80px;"
                                        "min-height: 25px;"
                              "}"

                              "QPushButton:pressed {"
                                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                             "stop: 0 #e3e3e3, stop: 0.50 #FFFFFF,"
                                                             "stop: 0.51 #FFFFFF, stop: 1 #e3e3e3);"
                                        "color: #515151;"
                              "}");

    QVBoxLayout *vbox = new QVBoxLayout(this);
                 vbox->setAlignment(buttonSave, Qt::AlignRight);

    QHBoxLayout *hboxTitle = new QHBoxLayout(this);
                 hboxTitle->addWidget(title);

    QFrame *line = new QFrame(this);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);

    QHBoxLayout *hboxText = new QHBoxLayout(this);
                 hboxText->addWidget(image);
                 hboxText->addWidget(text);

    QHBoxLayout *hboxButton = new QHBoxLayout(this);
                 hboxButton->addWidget(buttonSave);
                 hboxButton->setAlignment(buttonSave, Qt::AlignCenter);

    vbox->addLayout(hboxTitle);
    vbox->addWidget(line);
    vbox->addLayout(hboxText);
    vbox->addLayout(hboxButton);

    setLayout(vbox);

    connect(buttonSave, SIGNAL(clicked()), SLOT(close()));
}

CCommunicate::~CCommunicate()
{
    delete solitaryCommunicate; solitaryCommunicate = nullptr;
}

void CCommunicate::window()
{
    centerOnApplication();
    show();
}

void CCommunicate::centerOnApplication()
{
    auto x = CLogisticMainWindow::instance()->geometry().center().x() - rect().center().x() - 4;
    auto y = CLogisticMainWindow::instance()->geometry().center().y() - rect().center().y() - 30;

    move(x, y);
}

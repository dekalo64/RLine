#include "source/crm_additionally/adl_communicate.h"
#include "source/crm_core/core_logisticapplication.h"
#include "source/crm_core/core_logisticmainwindow.h"

static CCommunicate *theSolitaryCommunicate = nullptr;

QDialog *CCommunicate::showing(const QString &text){
    theSolitaryCommunicate = new CCommunicate(text);
    theSolitaryCommunicate->window();
    return theSolitaryCommunicate;
}

void CCommunicate::hiding()
{
    if (!theSolitaryCommunicate)
        return;
    theSolitaryCommunicate->hide();
    delete theSolitaryCommunicate;
           theSolitaryCommunicate = nullptr;
}

CCommunicate::CCommunicate(const QString &message)
    : QDialog(LogisticApplication::instance()->mainWindow(),
              Qt::Window | Qt::CustomizeWindowHint)
{
    setObjectName("CCommunicate");

    setMaximumSize(QSize(280, 120));
    setMinimumSize(QSize(280, 120));

    QLabel      *title      = new QLabel("Ошибка", this);
                 QFont fontTitle(QFont("Arial", 11));
                       fontTitle.setBold (true);
                 title->setFont(fontTitle);

    QLabel      *image       = new QLabel(this);
                 image->setPixmap(QPixmap("data/picture/additionally/alert.png"));
                 image->setMaximumSize(QSize(32, 32));

    QLabel      *text       = new QLabel(message, this);
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
                              "color: #515151;"
                              "min-width: 80px;"
                              "min-height: 25px;"
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
    delete theSolitaryCommunicate;
           theSolitaryCommunicate = nullptr;
}

void CCommunicate::window()
{
    centerOnApplication();
    show();
}

void CCommunicate::centerOnApplication()
{
    move(LogisticApplication::instance()->mainWindow()->geometry().center().x() - rect().center().x(),
         LogisticApplication::instance()->mainWindow()->geometry().center().y() - rect().center().y());
}

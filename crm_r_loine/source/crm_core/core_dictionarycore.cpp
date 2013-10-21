#include "source/crm_core/core_dictionarycore.h"
#include "source/crm_core/core_logisticapplication.h"

const QString CDictionaryCore::currentUser(void)
{
    QList<QVariant> list;
    QSqlQuery       stored;
    QString         userName;

    list.append((int)-1);
    stored = this->execStored(currentDatabase(), "ReadCurrentUser", storageHashTable(list));
    while (stored.next()){
        userName = stored.record().field("opt_name_first").value().toString();
    }
    stored.finish();
    return userName;
}

const QHash<QString, QVariant> CDictionaryCore::storageHashTable(const QList<QVariant> &list)
{
    unsigned i(0);
    QHash <QString, QVariant> hash;

    foreach (const QVariant &value, list) {
        switch (value.type()) {
        case QVariant::String:
            hash.insert(QString("@param%1").arg(i), value.toString());
            break;
        case QVariant::Bool:
            hash.insert(QString("@param%1").arg(i), value.toBool());
            break;
        case QVariant::Int:
            hash.insert(QString("@param%1").arg(i), value.toInt());
            break;
        case QVariant::UInt:
            hash.insert(QString("@param%1").arg(i), value.toUInt());
            break;
        case QVariant::Invalid:
            hash.insert(QString("@param%1").arg(i), QVariant(QVariant::Int));
            break;
        default:
            break;
        }
        ++i;
    }
    return hash;
}

QSqlQuery CDictionaryCore::execStored(const QSqlDatabase &database, const QString &storedFunction,
                                           const QHash<QString, QVariant> &hash)
{
    QSqlQuery stored(database);
              stored.setForwardOnly(true);

    if (hash.isEmpty())
        stored.prepare(storedFunction);
    else
        stored.prepare(QString("EXECUTE %1 ?%2")
                       .arg(storedFunction)
                       .arg(QString(", ?").repeated(hash.size()-1)));
    for (QHash<QString, QVariant>::const_iterator i = hash.constBegin(); i != hash.constEnd(); ++i){
        stored.bindValue(i.key(), i.value(), QSql::In);
    }
    stored.exec();
    return stored;
}

const QSqlDatabase CDictionaryCore::currentDatabase()
{
    return CLogisticApplication::instance()->database;
}

void CDictionaryCore::clearEditDialog(QDialog *dialog)
{
    foreach (QWidget *widget, dialog->findChildren<QWidget*>()) {
        if (QLineEdit *le = qobject_cast<QLineEdit*>(widget)){
           le->clear();
        }
        if (QCheckBox *chb = qobject_cast<QCheckBox*>(widget)){
           chb->setChecked(false);
        }
        if (QComboBox *cmb = qobject_cast<QComboBox*>(widget)){
           cmb->setCurrentIndex(-1);
        }
        if (QTextEdit *txt = qobject_cast<QTextEdit*>(widget)){
           txt->clear();
        }
        if (QListWidget *lw = qobject_cast<QListWidget*>(widget)){
           lw->clear();
        }
    }
    dialog->accept();
}

CFilter::CFilter(QWidget *parent)
    : QLineEdit(parent)
{
    clearButton = new QToolButton(this);
    QPixmap pixmapClear("data/picture/additionally/clear-button.png");

    searchButton = new QToolButton(this);
    QPixmap pixmapSearch("data/picture/additionally/search.png");

    clearButton->setIcon(QIcon(pixmapClear));
    clearButton->setIconSize(pixmapClear.size());
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setStyleSheet("QToolButton {"
                                    "border: none;"
                                    "padding: 0px;"
                               "}"

                               "QToolButton:hover {"
                                    "border: none;"
                                    "border-radius: 0px;"
                                    "background-color: transparent;"
                               "}");
    clearButton->hide();

    searchButton->setIcon(QIcon(pixmapSearch));
    searchButton->setIconSize(pixmapSearch.size());
    searchButton->setStyleSheet("QToolButton {"
                                    "border: none;"
                                    "padding: 0px;"
                                "}"

                                "QToolButton:hover {"
                                    "border: none;"
                                    "border-radius: 0px;"
                                    "background-color: transparent;"
                                "}");

    connect(this, SIGNAL(textChanged(const QString&)), SLOT(slotUpdateCloseButton(const QString&)));
    connect(clearButton, SIGNAL(clicked()), SLOT(slotClearSearchToItem()));

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth + 1));
    setStyleSheet(QString("QLineEdit { padding-left: %1px; } ").arg(searchButton->sizeHint().width() + frameWidth + 1));
    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
    setMinimumSize(qMax(msz.width(), searchButton->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), searchButton->sizeHint().height() + frameWidth * 2 + 2));
}

CFilter::~CFilter()
{
}

void CFilter::resizeEvent(QResizeEvent *)
{
    QSize szClear = clearButton->sizeHint();
    QSize szSearch = searchButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    clearButton->move(rect().right() - frameWidth - szClear.width(),
                      (rect().bottom() + 1 - szClear.height()) / 2);
    searchButton->move(rect().left() + 4,
                       (rect().bottom() + 1 - szSearch.height()) / 2);
}

void CFilter::slotUpdateCloseButton(const QString &text)
{
    clearButton->setVisible(!text.isEmpty());
}

void CFilter::slotClearSearchToItem()
{
    clear();
}

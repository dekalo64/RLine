#include "ui_dct_suppliers.h"

#include "source/crm_dictionary/dct_suppliers.h"
#include "source/crm_additionally/adl_communicate.h"

#define  SUPPLIERS_MODEL_COLUMN_COUNT 4

QT_BEGIN_NAMESPACE
class QCoreApplication;
QT_END_NAMESPACE

Suppliers::Suppliers(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::Suppliers)
{
    ui->setupUi(this);

    m_suppliersModel = new QStandardItemModel(this);
    m_selectionSuppliersModel = new QItemSelectionModel(m_suppliersModel);

    ui->treeViewSuppliers->setModel(m_suppliersModel);
    ui->treeViewSuppliers->setSelectionModel(m_selectionSuppliersModel);
    ui->treeViewSuppliers->setEditTriggers(QAbstractItemView::NoEditTriggers);

    rootItem = new QStandardItem(QString("Поставщики").trimmed());
    rootItem->setIcon(QIcon("data/picture/additionally/catalog.ico"));
    m_suppliersModel->insertRow(0, rootItem);
    QFont m_font(ui->treeViewSuppliers->font());
    m_font.setBold (true);
    m_suppliersModel->setData(m_suppliersModel->index(0, 0), m_font, Qt::FontRole);

//    slotSelectRecordsSuppliers(true);
}

Suppliers::~Suppliers()
{
    if ( IS_VALID_PTR(m_selectionSuppliersModel) ) { delete m_selectionSuppliersModel; m_selectionSuppliersModel = nullptr; }
    if ( IS_VALID_PTR(m_suppliersModel) )          { delete m_suppliersModel;          m_suppliersModel          = nullptr; }
    if ( IS_VALID_PTR(ui) )                        { delete ui;                        ui                        = nullptr; }
}

void Suppliers::fillingSuppliersModel(QSqlQuery &stored)
{
//    QMultiMap<int, QString>      map_np;
//    QMultiMap<QString, QString>  map_nn;
//    QList <QStandardItem *>      list;

//    m_suppliersModel->clear();
//    m_suppliersModel->insertColumns(0, SUPPLIERS_MODEL_COLUMN_COUNT);
//    m_suppliersModel->insertRows(stored.numRowsAffected(), 0);

//#ifndef QT_NO_CURSOR
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//#endif

//    map_np = setTreeData (stored, rootItem); // parent : name
//    map_nn = namePerIndex(stored, map_np);

//    for (QMultiMap<QString, QString>::const_iterator i = map_nn.begin(); i != map_nn.end(); ++i){
//        if (list.size() > 0) {
//            list.clear();
//        }
//        list = m_suppliersModel->findItems(i.value(), Qt::MatchExactly | Qt::MatchRecursive);
//        QStandardItem *p;
//        if (list.size() > 0){
//            foreach (p, list) {
//                stored.first();
//                while(stored.next()){
//                    if (stored.value(stored.record().indexOf("cty_parent")).toInt() != 0 &&
//                            (stored.value(stored.record().indexOf("cty_name_first")).toString() == i.key())){
//                        setTreeValue(stored, p, p->rowCount());
//                    }
//                }
//            }
//        }
//    }
//#ifndef QT_NO_CURSOR
//    QApplication::restoreOverrideCursor();
//#endif
}

//QMultiMap<int, QString> Suppliers::setTreeData(QSqlQuery &stored, QStandardItem *p)
//{
//    QMultiMap<int, QString> map;

//    while(stored.next()){
//        if (stored.value(stored.record().indexOf("cty_parent")).toInt() == 0){
//            setTreeValue(stored, p, p->rowCount());
//        } else {
//            map.insert(
//                        stored.value(stored.record().indexOf("cty_parent")).toInt(),
//                        stored.value(stored.record().indexOf("cty_name_first")).toString());
//        }
//    }
//  return map;
//}

//void Suppliers::setTreeValue(QSqlQuery &stored, QStandardItem *p, int row)
//{
//    unsigned ncols = stored.record().count();

//    for (unsigned i = 0; i != ncols; ++i){
//        switch (i) {
//        case 0:
//            p->setChild(row, 1, new QStandardItem(stored.value(i).toString()));
//            break;
//        case 1:
//            p->setChild(row, 5, new QStandardItem(stored.value(i).toString()));
//            break;
//        case 2:
//            p->setChild(row, 0, new QStandardItem(QIcon("data/picture/additionally/catalog.ico"), stored.value(i).toString()));
//            break;
//        case 3:
//            p->setChild(row, 2, new QStandardItem(stored.value(i).toString()));
//            break;
//        case 4:
//            p->setChild(row, 3, new QStandardItem(stored.value(i).toString()));
//            break;
//        case 5:
//            p->setChild(row, 4, new QStandardItem(stored.value(i).toString()));
//            break;
//        default:
//            break;
//        }
//    }
//}

//QMultiMap<QString, QString> Suppliers::namePerIndex(QSqlQuery &stored, QMultiMap<int, QString> &m)
//{
//    QMultiMap<QString, QString> map;

//    stored.first();
//    while (stored.next()){
//        for (QMultiMap<int, QString>::const_iterator i = m.begin(); i != m.end(); ++i){
//            if (stored.value(stored.record().indexOf("cty_code")).toInt() == i.key()){
//                map.insert(i.value(), stored.value(stored.record().indexOf("cty_name_first")).toString()); // name : parent name
//            }
//        }

//    }
//    return map;
//}

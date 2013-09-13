/****************************************************************************
** Meta object code from reading C++ file 'dct_customer.h'
**
** Created: Fri 13. Sep 00:52:03 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dictionary/dct_customer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dct_customer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCustomer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x08,
      44,   11,   10,   10, 0x08,
      73,   11,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCustomer[] = {
    "CCustomer\0\0index\0slotFillGroup(QModelIndex)\0"
    "slotFillPartner(QModelIndex)\0"
    "slotClearGroup(QModelIndex)\0"
};

void CCustomer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCustomer *_t = static_cast<CCustomer *>(_o);
        switch (_id) {
        case 0: _t->slotFillGroup((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->slotFillPartner((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->slotClearGroup((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCustomer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCustomer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCustomer,
      qt_meta_data_CCustomer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCustomer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCustomer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCustomer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCustomer))
        return static_cast<void*>(const_cast< CCustomer*>(this));
    if (!strcmp(_clname, "CDictionaryCore"))
        return static_cast< CDictionaryCore*>(const_cast< CCustomer*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCustomer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_CCustomerTreeView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_CCustomerTreeView[] = {
    "CCustomerTreeView\0"
};

void CCustomerTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CCustomerTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCustomerTreeView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_CCustomerTreeView,
      qt_meta_data_CCustomerTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCustomerTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCustomerTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCustomerTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCustomerTreeView))
        return static_cast<void*>(const_cast< CCustomerTreeView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int CCustomerTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

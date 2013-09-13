/****************************************************************************
** Meta object code from reading C++ file 'dct_cppsst.h'
**
** Created: Fri 13. Sep 00:52:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dictionary/dct_cppsst.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dct_cppsst.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCppsst[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x09,

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

static const char qt_meta_stringdata_CCppsst[] = {
    "CCppsst\0\0r\0slotCreateEditDialog(int)\0"
};

void CCppsst::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCppsst *_t = static_cast<CCppsst *>(_o);
        switch (_id) {
        case 0: _t->slotCreateEditDialog((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCppsst::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCppsst::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCppsst,
      qt_meta_data_CCppsst, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCppsst::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCppsst::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCppsst::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCppsst))
        return static_cast<void*>(const_cast< CCppsst*>(this));
    if (!strcmp(_clname, "CDictionaryCore"))
        return static_cast< CDictionaryCore*>(const_cast< CCppsst*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCppsst::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_CTreeViewCppsst[] = {

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

static const char qt_meta_stringdata_CTreeViewCppsst[] = {
    "CTreeViewCppsst\0"
};

void CTreeViewCppsst::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CTreeViewCppsst::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTreeViewCppsst::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_CTreeViewCppsst,
      qt_meta_data_CTreeViewCppsst, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTreeViewCppsst::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTreeViewCppsst::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTreeViewCppsst::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTreeViewCppsst))
        return static_cast<void*>(const_cast< CTreeViewCppsst*>(this));
    return QTreeView::qt_metacast(_clname);
}

int CTreeViewCppsst::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

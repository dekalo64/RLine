/****************************************************************************
** Meta object code from reading C++ file 'core_dictionarycore.h'
**
** Created: Fri 13. Sep 00:52:02 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_core/core_dictionarycore.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'core_dictionarycore.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CFilter[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    9,    8,    8, 0x08,
      45,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CFilter[] = {
    "CFilter\0\0text\0slotUpdateCloseButton(QString)\0"
    "slotClearSearchToItem()\0"
};

void CFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CFilter *_t = static_cast<CFilter *>(_o);
        switch (_id) {
        case 0: _t->slotUpdateCloseButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->slotClearSearchToItem(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CFilter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CFilter::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_CFilter,
      qt_meta_data_CFilter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFilter))
        return static_cast<void*>(const_cast< CFilter*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int CFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

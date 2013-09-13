/****************************************************************************
** Meta object code from reading C++ file 'dct_priorities.h'
**
** Created: Fri 13. Sep 00:52:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dictionary/dct_priorities.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dct_priorities.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Priorities[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      35,   33,   11,   11, 0x08,
      68,   61,   11,   11, 0x08,
      97,   92,   11,   11, 0x08,
     125,   11,   11,   11, 0x08,
     153,   11,   11,   11, 0x08,
     171,   11,   11,   11, 0x08,
     191,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Priorities[] = {
    "Priorities\0\0slotFillPriorities()\0r\0"
    "slotCreateEditDialog(int)\0actual\0"
    "slotActualRecords(bool)\0text\0"
    "slotFindPriorities(QString)\0"
    "slotInsertOrUpdateRecords()\0"
    "slotCopyRecords()\0slotDeleteRecords()\0"
    "slotRefreshRecords()\0"
};

void Priorities::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Priorities *_t = static_cast<Priorities *>(_o);
        switch (_id) {
        case 0: _t->slotFillPriorities(); break;
        case 1: _t->slotCreateEditDialog((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: _t->slotActualRecords((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 3: _t->slotFindPriorities((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->slotInsertOrUpdateRecords(); break;
        case 5: _t->slotCopyRecords(); break;
        case 6: _t->slotDeleteRecords(); break;
        case 7: _t->slotRefreshRecords(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Priorities::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Priorities::staticMetaObject = {
    { &CCppsst::staticMetaObject, qt_meta_stringdata_Priorities,
      qt_meta_data_Priorities, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Priorities::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Priorities::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Priorities::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Priorities))
        return static_cast<void*>(const_cast< Priorities*>(this));
    return CCppsst::qt_metacast(_clname);
}

int Priorities::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CCppsst::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dct_task.h'
**
** Created: Fri 13. Sep 00:52:04 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dictionary/dct_task.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dct_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaskType[] = {

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
      10,    9,    9,    9, 0x08,
      27,   25,    9,    9, 0x08,
      60,   53,    9,    9, 0x08,
      89,   84,    9,    9, 0x08,
     111,    9,    9,    9, 0x08,
     139,    9,    9,    9, 0x08,
     157,    9,    9,    9, 0x08,
     177,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TaskType[] = {
    "TaskType\0\0slotFillTask()\0r\0"
    "slotCreateEditDialog(int)\0actual\0"
    "slotActualRecords(bool)\0text\0"
    "slotFindTask(QString)\0slotInsertOrUpdateRecords()\0"
    "slotCopyRecords()\0slotDeleteRecords()\0"
    "slotRefreshRecords()\0"
};

void TaskType::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TaskType *_t = static_cast<TaskType *>(_o);
        switch (_id) {
        case 0: _t->slotFillTask(); break;
        case 1: _t->slotCreateEditDialog((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: _t->slotActualRecords((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 3: _t->slotFindTask((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->slotInsertOrUpdateRecords(); break;
        case 5: _t->slotCopyRecords(); break;
        case 6: _t->slotDeleteRecords(); break;
        case 7: _t->slotRefreshRecords(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TaskType::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TaskType::staticMetaObject = {
    { &CCppsst::staticMetaObject, qt_meta_stringdata_TaskType,
      qt_meta_data_TaskType, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskType::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskType::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskType::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskType))
        return static_cast<void*>(const_cast< TaskType*>(this));
    return CCppsst::qt_metacast(_clname);
}

int TaskType::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

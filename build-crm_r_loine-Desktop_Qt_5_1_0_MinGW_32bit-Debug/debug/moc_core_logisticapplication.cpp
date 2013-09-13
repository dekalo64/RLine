/****************************************************************************
** Meta object code from reading C++ file 'core_logisticapplication.h'
**
** Created: Fri 13. Sep 00:52:23 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_core/core_logisticapplication.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'core_logisticapplication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LogisticApplication[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   51,   41,   20, 0x08,
     106,   98,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LogisticApplication[] = {
    "LogisticApplication\0\0successConnection()\0"
    "QSqlError\0user,passwd\0"
    "slotAddConnection(QString,QString)\0"
    "visible\0slotVisibleComponent(bool)\0"
};

void LogisticApplication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LogisticApplication *_t = static_cast<LogisticApplication *>(_o);
        switch (_id) {
        case 0: _t->successConnection(); break;
        case 1: { QSqlError _r = _t->slotAddConnection((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QSqlError*>(_a[0]) = _r; }  break;
        case 2: _t->slotVisibleComponent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LogisticApplication::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LogisticApplication::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_LogisticApplication,
      qt_meta_data_LogisticApplication, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LogisticApplication::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LogisticApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LogisticApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LogisticApplication))
        return static_cast<void*>(const_cast< LogisticApplication*>(this));
    return QApplication::qt_metacast(_clname);
}

int LogisticApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void LogisticApplication::successConnection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dlg_connection.h'
**
** Created: Fri 13. Sep 00:52:11 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dialog/dlg_connection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlg_connection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConnectionDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   18,   17,   17, 0x05,
      75,   67,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   17,   17,   17, 0x08,
     123,   17,   17,   17, 0x08,
     145,   17,   17,   17, 0x08,
     162,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConnectionDialog[] = {
    "ConnectionDialog\0\0user,passwd\0"
    "sendUserInformation(QString,QString)\0"
    "visible\0setVisibleComponent(bool)\0"
    "slotExitApplication()\0slotDataBaseConnect()\0"
    "currentChanged()\0slotSuccessConnection()\0"
};

void ConnectionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ConnectionDialog *_t = static_cast<ConnectionDialog *>(_o);
        switch (_id) {
        case 0: _t->sendUserInformation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->setVisibleComponent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slotExitApplication(); break;
        case 3: _t->slotDataBaseConnect(); break;
        case 4: _t->currentChanged(); break;
        case 5: _t->slotSuccessConnection(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ConnectionDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ConnectionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ConnectionDialog,
      qt_meta_data_ConnectionDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConnectionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConnectionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConnectionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionDialog))
        return static_cast<void*>(const_cast< ConnectionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ConnectionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ConnectionDialog::sendUserInformation(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConnectionDialog::setVisibleComponent(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

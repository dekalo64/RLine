/****************************************************************************
** Meta object code from reading C++ file 'dlg_city.h'
**
** Created: Fri 13. Sep 00:52:02 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dialog/dlg_city.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlg_city.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CityDialog[] = {

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
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   11,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CityDialog[] = {
    "CityDialog\0\0saveDataChanged()\0"
    "slotCurrentChanged()\0slotSaveDataChanged()\0"
};

void CityDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CityDialog *_t = static_cast<CityDialog *>(_o);
        switch (_id) {
        case 0: _t->saveDataChanged(); break;
        case 1: _t->slotCurrentChanged(); break;
        case 2: _t->slotSaveDataChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CityDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CityDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CityDialog,
      qt_meta_data_CityDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CityDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CityDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CityDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CityDialog))
        return static_cast<void*>(const_cast< CityDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CityDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void CityDialog::saveDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

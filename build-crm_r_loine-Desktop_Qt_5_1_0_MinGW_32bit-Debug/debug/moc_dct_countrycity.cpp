/****************************************************************************
** Meta object code from reading C++ file 'dct_countrycity.h'
**
** Created: Fri 13. Sep 00:52:23 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../crm_r_loine/source/crm_dictionary/dct_countrycity.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dct_countrycity.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCountryCity[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x08,
      47,   14,   13,   13, 0x08,
      75,   14,   13,   13, 0x08,
     110,  103,   13,   13, 0x08,
     139,  134,   13,   13, 0x08,
     163,   13,   13,   13, 0x08,
     180,   13,   13,   13, 0x08,
     198,   13,   13,   13, 0x08,
     217,   13,   13,   13, 0x08,
     237,   13,   13,   13, 0x08,
     258,   13,   13,   13, 0x08,
     286,   13,   13,   13, 0x08,
     313,  311,   13,   13, 0x08,
     339,   13,   13,   13, 0x08,

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

static const char qt_meta_stringdata_CCountryCity[] = {
    "CCountryCity\0\0index\0slotFillGroup(QModelIndex)\0"
    "slotFillCities(QModelIndex)\0"
    "slotClearGroup(QModelIndex)\0actual\0"
    "slotActualRecords(bool)\0text\0"
    "slotFindCities(QString)\0slotCutRecords()\0"
    "slotCopyRecords()\0slotPasteRecords()\0"
    "slotDeleteRecords()\0slotRefreshRecords()\0"
    "slotRefreshRecordsCountry()\0"
    "slotRefreshRecordsCity()\0r\0"
    "slotCreateEditDialog(int)\0"
    "slotInsertOrUpdateRecords()\0"
};

void CCountryCity::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCountryCity *_t = static_cast<CCountryCity *>(_o);
        switch (_id) {
        case 0: _t->slotFillGroup((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->slotFillCities((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->slotClearGroup((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->slotActualRecords((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 4: _t->slotFindCities((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->slotCutRecords(); break;
        case 6: _t->slotCopyRecords(); break;
        case 7: _t->slotPasteRecords(); break;
        case 8: _t->slotDeleteRecords(); break;
        case 9: _t->slotRefreshRecords(); break;
        case 10: _t->slotRefreshRecordsCountry(); break;
        case 11: _t->slotRefreshRecordsCity(); break;
        case 12: _t->slotCreateEditDialog((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 13: _t->slotInsertOrUpdateRecords(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCountryCity::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCountryCity::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCountryCity,
      qt_meta_data_CCountryCity, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCountryCity::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCountryCity::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCountryCity::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCountryCity))
        return static_cast<void*>(const_cast< CCountryCity*>(this));
    if (!strcmp(_clname, "CDictionaryCore"))
        return static_cast< CDictionaryCore*>(const_cast< CCountryCity*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCountryCity::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
static const uint qt_meta_data_CCityTreeView[] = {

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

static const char qt_meta_stringdata_CCityTreeView[] = {
    "CCityTreeView\0"
};

void CCityTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CCityTreeView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCityTreeView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_CCityTreeView,
      qt_meta_data_CCityTreeView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCityTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCityTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCityTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCityTreeView))
        return static_cast<void*>(const_cast< CCityTreeView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int CCityTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

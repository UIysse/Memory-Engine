/****************************************************************************
** Meta object code from reading C++ file 'GlobalSearchHeader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GlobalSearchHeader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GlobalSearchHeader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GlobalSearchInstance_t {
    QByteArrayData data[11];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GlobalSearchInstance_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GlobalSearchInstance_t qt_meta_stringdata_GlobalSearchInstance = {
    {
QT_MOC_LITERAL(0, 0, 20), // "GlobalSearchInstance"
QT_MOC_LITERAL(1, 21, 16), // "signalItemChange"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 56, 3), // "itm"
QT_MOC_LITERAL(5, 60, 6), // "nValue"
QT_MOC_LITERAL(6, 67, 6), // "column"
QT_MOC_LITERAL(7, 74, 12), // "bValidMemory"
QT_MOC_LITERAL(8, 87, 11), // "AddVariable"
QT_MOC_LITERAL(9, 99, 10), // "AddComment"
QT_MOC_LITERAL(10, 110, 9) // "SetValues"

    },
    "GlobalSearchInstance\0signalItemChange\0"
    "\0QTreeWidgetItem*\0itm\0nValue\0column\0"
    "bValidMemory\0AddVariable\0AddComment\0"
    "SetValues"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GlobalSearchInstance[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   43,    2, 0x0a /* Public */,
       9,    2,   48,    2, 0x0a /* Public */,
      10,    4,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong, QMetaType::Int, QMetaType::Bool,    4,    5,    6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    6,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    6,
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong, QMetaType::Int, QMetaType::Bool,    4,    5,    6,    7,

       0        // eod
};

void GlobalSearchInstance::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GlobalSearchInstance *_t = static_cast<GlobalSearchInstance *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalItemChange((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< unsigned long long(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->AddVariable((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->AddComment((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->SetValues((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< unsigned long long(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GlobalSearchInstance::*_t)(QTreeWidgetItem * , unsigned long long , int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GlobalSearchInstance::signalItemChange)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GlobalSearchInstance::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GlobalSearchInstance.data,
      qt_meta_data_GlobalSearchInstance,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GlobalSearchInstance::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlobalSearchInstance::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GlobalSearchInstance.stringdata0))
        return static_cast<void*>(const_cast< GlobalSearchInstance*>(this));
    return QObject::qt_metacast(_clname);
}

int GlobalSearchInstance::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GlobalSearchInstance::signalItemChange(QTreeWidgetItem * _t1, unsigned long long _t2, int _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

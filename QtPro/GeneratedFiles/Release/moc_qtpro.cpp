/****************************************************************************
** Meta object code from reading C++ file 'qtpro.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtpro.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtpro.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_About_t {
    QByteArrayData data[1];
    char stringdata0[6];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_About_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_About_t qt_meta_stringdata_About = {
    {
QT_MOC_LITERAL(0, 0, 5) // "About"

    },
    "About"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_About[] = {

 // content:
       7,       // revision
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

void About::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject About::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_About.data,
      qt_meta_data_About,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *About::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *About::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_About.stringdata0))
        return static_cast<void*>(const_cast< About*>(this));
    return QDialog::qt_metacast(_clname);
}

int About::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_QtPro_t {
    QByteArrayData data[12];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtPro_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtPro_t qt_meta_stringdata_QtPro = {
    {
QT_MOC_LITERAL(0, 0, 5), // "QtPro"
QT_MOC_LITERAL(1, 6, 15), // "ShowAboutDialog"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 15), // "ShowPickProcess"
QT_MOC_LITERAL(4, 39, 14), // "ShowMemoryView"
QT_MOC_LITERAL(5, 54, 8), // "ShowLogs"
QT_MOC_LITERAL(6, 63, 10), // "ShowSearch"
QT_MOC_LITERAL(7, 74, 11), // "ShowModules"
QT_MOC_LITERAL(8, 86, 17), // "UpdateProcessName"
QT_MOC_LITERAL(9, 104, 10), // "closeEvent"
QT_MOC_LITERAL(10, 115, 12), // "QCloseEvent*"
QT_MOC_LITERAL(11, 128, 5) // "event"

    },
    "QtPro\0ShowAboutDialog\0\0ShowPickProcess\0"
    "ShowMemoryView\0ShowLogs\0ShowSearch\0"
    "ShowModules\0UpdateProcessName\0closeEvent\0"
    "QCloseEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtPro[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void QtPro::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtPro *_t = static_cast<QtPro *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowAboutDialog(); break;
        case 1: _t->ShowPickProcess(); break;
        case 2: _t->ShowMemoryView(); break;
        case 3: _t->ShowLogs(); break;
        case 4: _t->ShowSearch(); break;
        case 5: _t->ShowModules(); break;
        case 6: _t->UpdateProcessName(); break;
        case 7: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QtPro::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QtPro.data,
      qt_meta_data_QtPro,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QtPro::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtPro::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtPro.stringdata0))
        return static_cast<void*>(const_cast< QtPro*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QtPro::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
struct qt_meta_stringdata_MemoryViewer_t {
    QByteArrayData data[4];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MemoryViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MemoryViewer_t qt_meta_stringdata_MemoryViewer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MemoryViewer"
QT_MOC_LITERAL(1, 13, 8), // "showDlls"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 17) // "ShowGotoDialogBox"

    },
    "MemoryViewer\0showDlls\0\0ShowGotoDialogBox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MemoryViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MemoryViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MemoryViewer *_t = static_cast<MemoryViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showDlls(); break;
        case 1: _t->ShowGotoDialogBox(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MemoryViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MemoryViewer.data,
      qt_meta_data_MemoryViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MemoryViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MemoryViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MemoryViewer.stringdata0))
        return static_cast<void*>(const_cast< MemoryViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MemoryViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_InputGotoBox_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InputGotoBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InputGotoBox_t qt_meta_stringdata_InputGotoBox = {
    {
QT_MOC_LITERAL(0, 0, 12) // "InputGotoBox"

    },
    "InputGotoBox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InputGotoBox[] = {

 // content:
       7,       // revision
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

void InputGotoBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject InputGotoBox::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_InputGotoBox.data,
      qt_meta_data_InputGotoBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *InputGotoBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InputGotoBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_InputGotoBox.stringdata0))
        return static_cast<void*>(const_cast< InputGotoBox*>(this));
    return QDialog::qt_metacast(_clname);
}

int InputGotoBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

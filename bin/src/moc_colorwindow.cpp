/****************************************************************************
** Meta object code from reading C++ file 'colorwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "C:/Users/yxing/Documents/workspace/colorlevels/colorwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'colorwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ColorConstraint_t {
    QByteArrayData data[14];
    char stringdata[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ColorConstraint_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ColorConstraint_t qt_meta_stringdata_ColorConstraint = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ColorConstraint"
QT_MOC_LITERAL(1, 16, 7), // "updated"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 6), // "remove"
QT_MOC_LITERAL(4, 32, 16), // "ColorConstraint*"
QT_MOC_LITERAL(5, 49, 13), // "toggleVisible"
QT_MOC_LITERAL(6, 63, 10), // "emitRemove"
QT_MOC_LITERAL(7, 74, 17), // "sliderTextChanged"
QT_MOC_LITERAL(8, 92, 4), // "text"
QT_MOC_LITERAL(9, 97, 11), // "sliderMoved"
QT_MOC_LITERAL(10, 109, 11), // "sliderValue"
QT_MOC_LITERAL(11, 121, 15), // "preventDebounce"
QT_MOC_LITERAL(12, 137, 18), // "thresholdDebounced"
QT_MOC_LITERAL(13, 156, 16) // "thresholdUpdated"

    },
    "ColorConstraint\0updated\0\0remove\0"
    "ColorConstraint*\0toggleVisible\0"
    "emitRemove\0sliderTextChanged\0text\0"
    "sliderMoved\0sliderValue\0preventDebounce\0"
    "thresholdDebounced\0thresholdUpdated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColorConstraint[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   63,    2, 0x09 /* Protected */,
       6,    0,   64,    2, 0x09 /* Protected */,
       7,    1,   65,    2, 0x09 /* Protected */,
       9,    1,   68,    2, 0x09 /* Protected */,
      11,    0,   71,    2, 0x09 /* Protected */,
      12,    0,   72,    2, 0x09 /* Protected */,
      13,    0,   73,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ColorConstraint::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColorConstraint *_t = static_cast<ColorConstraint *>(_o);
        switch (_id) {
        case 0: _t->updated(); break;
        case 1: _t->remove((*reinterpret_cast< ColorConstraint*(*)>(_a[1]))); break;
        case 2: _t->toggleVisible(); break;
        case 3: _t->emitRemove(); break;
        case 4: _t->sliderTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->preventDebounce(); break;
        case 7: _t->thresholdDebounced(); break;
        case 8: _t->thresholdUpdated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ColorConstraint* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ColorConstraint::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColorConstraint::updated)) {
                *result = 0;
            }
        }
        {
            typedef void (ColorConstraint::*_t)(ColorConstraint * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColorConstraint::remove)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ColorConstraint::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ColorConstraint.data,
      qt_meta_data_ColorConstraint,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ColorConstraint::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorConstraint::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ColorConstraint.stringdata))
        return static_cast<void*>(const_cast< ColorConstraint*>(this));
    return QWidget::qt_metacast(_clname);
}

int ColorConstraint::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ColorConstraint::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ColorConstraint::remove(ColorConstraint * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_OverviewWidget_t {
    QByteArrayData data[1];
    char stringdata[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OverviewWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OverviewWidget_t qt_meta_stringdata_OverviewWidget = {
    {
QT_MOC_LITERAL(0, 0, 14) // "OverviewWidget"

    },
    "OverviewWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OverviewWidget[] = {

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

void OverviewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject OverviewWidget::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_OverviewWidget.data,
      qt_meta_data_OverviewWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OverviewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OverviewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OverviewWidget.stringdata))
        return static_cast<void*>(const_cast< OverviewWidget*>(this));
    return QLabel::qt_metacast(_clname);
}

int OverviewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_ColorWindow_t {
    QByteArrayData data[9];
    char stringdata[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ColorWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ColorWindow_t qt_meta_stringdata_ColorWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ColorWindow"
QT_MOC_LITERAL(1, 12, 18), // "constraintsUpdated"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "removeConstraint"
QT_MOC_LITERAL(4, 49, 16), // "ColorConstraint*"
QT_MOC_LITERAL(5, 66, 1), // "c"
QT_MOC_LITERAL(6, 68, 16), // "clearConstraints"
QT_MOC_LITERAL(7, 85, 11), // "exportImage"
QT_MOC_LITERAL(8, 97, 14) // "updateOverview"

    },
    "ColorWindow\0constraintsUpdated\0\0"
    "removeConstraint\0ColorConstraint*\0c\0"
    "clearConstraints\0exportImage\0"
    "updateOverview"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColorWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    1,   40,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,
       7,    0,   44,    2, 0x0a /* Public */,
       8,    0,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ColorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColorWindow *_t = static_cast<ColorWindow *>(_o);
        switch (_id) {
        case 0: _t->constraintsUpdated(); break;
        case 1: _t->removeConstraint((*reinterpret_cast< ColorConstraint*(*)>(_a[1]))); break;
        case 2: _t->clearConstraints(); break;
        case 3: _t->exportImage(); break;
        case 4: _t->updateOverview(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ColorConstraint* >(); break;
            }
            break;
        }
    }
}

const QMetaObject ColorWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ColorWindow.data,
      qt_meta_data_ColorWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ColorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ColorWindow.stringdata))
        return static_cast<void*>(const_cast< ColorWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ColorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

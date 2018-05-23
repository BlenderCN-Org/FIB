/****************************************************************************
** Meta object code from reading C++ file 'separable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FRR/separable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'separable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_separable_t {
    QByteArrayData data[8];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_separable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_separable_t qt_meta_stringdata_separable = {
    {
QT_MOC_LITERAL(0, 0, 9), // "separable"
QT_MOC_LITERAL(1, 10, 12), // "SetFramerate"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 9), // "setRadius"
QT_MOC_LITERAL(4, 34, 14), // "setFirstSample"
QT_MOC_LITERAL(5, 49, 15), // "setSecondSample"
QT_MOC_LITERAL(6, 65, 14), // "setThirdSample"
QT_MOC_LITERAL(7, 80, 15) // "setFourthSample"

    },
    "separable\0SetFramerate\0\0setRadius\0"
    "setFirstSample\0setSecondSample\0"
    "setThirdSample\0setFourthSample"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_separable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   47,    2, 0x0a /* Public */,
       4,    0,   50,    2, 0x0a /* Public */,
       5,    0,   51,    2, 0x0a /* Public */,
       6,    0,   52,    2, 0x0a /* Public */,
       7,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void separable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        separable *_t = static_cast<separable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetFramerate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setRadius((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setFirstSample(); break;
        case 3: _t->setSecondSample(); break;
        case 4: _t->setThirdSample(); break;
        case 5: _t->setFourthSample(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (separable::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&separable::SetFramerate)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject separable::staticMetaObject = {
    { &Viewer1::staticMetaObject, qt_meta_stringdata_separable.data,
      qt_meta_data_separable,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *separable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *separable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_separable.stringdata0))
        return static_cast<void*>(this);
    return Viewer1::qt_metacast(_clname);
}

int separable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Viewer1::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void separable::SetFramerate(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

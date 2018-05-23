/****************************************************************************
** Meta object code from reading C++ file 'ssao_viewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FRR/ssao_viewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ssao_viewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ssao_viewer_t {
    QByteArrayData data[11];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ssao_viewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ssao_viewer_t qt_meta_stringdata_ssao_viewer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ssao_viewer"
QT_MOC_LITERAL(1, 12, 12), // "SetFramerate"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "setRadius"
QT_MOC_LITERAL(4, 36, 13), // "setBlurRadius"
QT_MOC_LITERAL(5, 50, 9), // "setBlurOn"
QT_MOC_LITERAL(6, 60, 10), // "setBlurOff"
QT_MOC_LITERAL(7, 71, 14), // "setFirstSample"
QT_MOC_LITERAL(8, 86, 15), // "setSecondSample"
QT_MOC_LITERAL(9, 102, 14), // "setThirdSample"
QT_MOC_LITERAL(10, 117, 15) // "setFourthSample"

    },
    "ssao_viewer\0SetFramerate\0\0setRadius\0"
    "setBlurRadius\0setBlurOn\0setBlurOff\0"
    "setFirstSample\0setSecondSample\0"
    "setThirdSample\0setFourthSample"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ssao_viewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   62,    2, 0x0a /* Public */,
       4,    1,   65,    2, 0x0a /* Public */,
       5,    0,   68,    2, 0x0a /* Public */,
       6,    0,   69,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    0,   71,    2, 0x0a /* Public */,
       9,    0,   72,    2, 0x0a /* Public */,
      10,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ssao_viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ssao_viewer *_t = static_cast<ssao_viewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetFramerate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setRadius((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setBlurRadius((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setBlurOn(); break;
        case 4: _t->setBlurOff(); break;
        case 5: _t->setFirstSample(); break;
        case 6: _t->setSecondSample(); break;
        case 7: _t->setThirdSample(); break;
        case 8: _t->setFourthSample(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ssao_viewer::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ssao_viewer::SetFramerate)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ssao_viewer::staticMetaObject = {
    { &Viewer1::staticMetaObject, qt_meta_stringdata_ssao_viewer.data,
      qt_meta_data_ssao_viewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ssao_viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ssao_viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ssao_viewer.stringdata0))
        return static_cast<void*>(this);
    return Viewer1::qt_metacast(_clname);
}

int ssao_viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Viewer1::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ssao_viewer::SetFramerate(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

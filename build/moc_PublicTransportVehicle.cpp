/****************************************************************************
** Meta object code from reading C++ file 'PublicTransportVehicle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/vehicles/public_transport/PublicTransportVehicle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PublicTransportVehicle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PublicTransportVehicle_t {
    QByteArrayData data[9];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PublicTransportVehicle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PublicTransportVehicle_t qt_meta_stringdata_PublicTransportVehicle = {
    {
QT_MOC_LITERAL(0, 0, 22), // "PublicTransportVehicle"
QT_MOC_LITERAL(1, 23, 17), // "reachedStopSignal"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 29), // "PublicTransportVehicleDriver*"
QT_MOC_LITERAL(4, 72, 6), // "driver"
QT_MOC_LITERAL(5, 79, 23), // "PublicTransportVehicle*"
QT_MOC_LITERAL(6, 103, 7), // "vehicle"
QT_MOC_LITERAL(7, 111, 20), // "PublicTransportStop*"
QT_MOC_LITERAL(8, 132, 12) // "reached_stop"

    },
    "PublicTransportVehicle\0reachedStopSignal\0"
    "\0PublicTransportVehicleDriver*\0driver\0"
    "PublicTransportVehicle*\0vehicle\0"
    "PublicTransportStop*\0reached_stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PublicTransportVehicle[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7,    4,    6,    8,

       0        // eod
};

void PublicTransportVehicle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PublicTransportVehicle *_t = static_cast<PublicTransportVehicle *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reachedStopSignal((*reinterpret_cast< PublicTransportVehicleDriver*(*)>(_a[1])),(*reinterpret_cast< PublicTransportVehicle*(*)>(_a[2])),(*reinterpret_cast< PublicTransportStop*(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PublicTransportStop* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PublicTransportVehicle* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PublicTransportVehicleDriver* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PublicTransportVehicle::*_t)(PublicTransportVehicleDriver * , PublicTransportVehicle * , PublicTransportStop * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PublicTransportVehicle::reachedStopSignal)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject PublicTransportVehicle::staticMetaObject = {
    { &Vehicle::staticMetaObject, qt_meta_stringdata_PublicTransportVehicle.data,
      qt_meta_data_PublicTransportVehicle,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PublicTransportVehicle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PublicTransportVehicle::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PublicTransportVehicle.stringdata0))
        return static_cast<void*>(const_cast< PublicTransportVehicle*>(this));
    return Vehicle::qt_metacast(_clname);
}

int PublicTransportVehicle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vehicle::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void PublicTransportVehicle::reachedStopSignal(PublicTransportVehicleDriver * _t1, PublicTransportVehicle * _t2, PublicTransportStop * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

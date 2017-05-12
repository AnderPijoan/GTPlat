/****************************************************************************
** Meta object code from reading C++ file 'ItineraryVehicleDriver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/people/vehicle_drivers/ItineraryVehicleDriver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ItineraryVehicleDriver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ItineraryVehicleDriver_t {
    QByteArrayData data[7];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ItineraryVehicleDriver_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ItineraryVehicleDriver_t qt_meta_stringdata_ItineraryVehicleDriver = {
    {
QT_MOC_LITERAL(0, 0, 22), // "ItineraryVehicleDriver"
QT_MOC_LITERAL(1, 23, 6), // "behave"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 24), // "getInsideVehicleAndDrive"
QT_MOC_LITERAL(4, 56, 25), // "calculateRouteToNextPoint"
QT_MOC_LITERAL(5, 82, 20), // "stopAtItineraryPoint"
QT_MOC_LITERAL(6, 103, 24) // "departFromItineraryPoint"

    },
    "ItineraryVehicleDriver\0behave\0\0"
    "getInsideVehicleAndDrive\0"
    "calculateRouteToNextPoint\0"
    "stopAtItineraryPoint\0departFromItineraryPoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ItineraryVehicleDriver[] = {

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
       1,    0,   39,    2, 0x09 /* Protected */,
       3,    0,   40,    2, 0x09 /* Protected */,
       4,    0,   41,    2, 0x09 /* Protected */,
       5,    0,   42,    2, 0x09 /* Protected */,
       6,    0,   43,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ItineraryVehicleDriver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ItineraryVehicleDriver *_t = static_cast<ItineraryVehicleDriver *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->behave(); break;
        case 1: _t->getInsideVehicleAndDrive(); break;
        case 2: _t->calculateRouteToNextPoint(); break;
        case 3: _t->stopAtItineraryPoint(); break;
        case 4: _t->departFromItineraryPoint(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ItineraryVehicleDriver::staticMetaObject = {
    { &VehicleDriver::staticMetaObject, qt_meta_stringdata_ItineraryVehicleDriver.data,
      qt_meta_data_ItineraryVehicleDriver,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ItineraryVehicleDriver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ItineraryVehicleDriver::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ItineraryVehicleDriver.stringdata0))
        return static_cast<void*>(const_cast< ItineraryVehicleDriver*>(this));
    return VehicleDriver::qt_metacast(_clname);
}

int ItineraryVehicleDriver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VehicleDriver::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

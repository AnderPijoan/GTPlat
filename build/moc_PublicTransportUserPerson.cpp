/****************************************************************************
** Meta object code from reading C++ file 'PublicTransportUserPerson.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/people/public_transport_users/PublicTransportUserPerson.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PublicTransportUserPerson.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PublicTransportUserPerson_t {
    QByteArrayData data[19];
    char stringdata0[336];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PublicTransportUserPerson_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PublicTransportUserPerson_t qt_meta_stringdata_PublicTransportUserPerson = {
    {
QT_MOC_LITERAL(0, 0, 25), // "PublicTransportUserPerson"
QT_MOC_LITERAL(1, 26, 12), // "agentStarted"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 8), // "planTrip"
QT_MOC_LITERAL(4, 49, 35), // "calculateRouteToPublicTranspo..."
QT_MOC_LITERAL(5, 85, 15), // "followWalkRoute"
QT_MOC_LITERAL(6, 101, 20), // "retryFollowWalkRoute"
QT_MOC_LITERAL(7, 122, 22), // "waitForPublicTransport"
QT_MOC_LITERAL(8, 145, 24), // "departingPublicTransport"
QT_MOC_LITERAL(9, 170, 29), // "PublicTransportVehicleDriver*"
QT_MOC_LITERAL(10, 200, 6), // "driver"
QT_MOC_LITERAL(11, 207, 23), // "PublicTransportVehicle*"
QT_MOC_LITERAL(12, 231, 7), // "vehicle"
QT_MOC_LITERAL(13, 239, 27), // "QList<PublicTransportStop*>"
QT_MOC_LITERAL(14, 267, 16), // "passing_by_stops"
QT_MOC_LITERAL(15, 284, 11), // "reachedStop"
QT_MOC_LITERAL(16, 296, 20), // "PublicTransportStop*"
QT_MOC_LITERAL(17, 317, 4), // "stop"
QT_MOC_LITERAL(18, 322, 13) // "routeFinished"

    },
    "PublicTransportUserPerson\0agentStarted\0"
    "\0planTrip\0calculateRouteToPublicTransportStop\0"
    "followWalkRoute\0retryFollowWalkRoute\0"
    "waitForPublicTransport\0departingPublicTransport\0"
    "PublicTransportVehicleDriver*\0driver\0"
    "PublicTransportVehicle*\0vehicle\0"
    "QList<PublicTransportStop*>\0"
    "passing_by_stops\0reachedStop\0"
    "PublicTransportStop*\0stop\0routeFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PublicTransportUserPerson[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x09 /* Protected */,
       3,    0,   60,    2, 0x09 /* Protected */,
       4,    0,   61,    2, 0x09 /* Protected */,
       5,    0,   62,    2, 0x09 /* Protected */,
       6,    0,   63,    2, 0x09 /* Protected */,
       7,    0,   64,    2, 0x09 /* Protected */,
       8,    3,   65,    2, 0x09 /* Protected */,
      15,    3,   72,    2, 0x09 /* Protected */,
      18,    0,   79,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11, 0x80000000 | 13,   10,   12,   14,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11, 0x80000000 | 16,   10,   12,   17,
    QMetaType::Void,

       0        // eod
};

void PublicTransportUserPerson::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PublicTransportUserPerson *_t = static_cast<PublicTransportUserPerson *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->agentStarted(); break;
        case 1: _t->planTrip(); break;
        case 2: _t->calculateRouteToPublicTransportStop(); break;
        case 3: _t->followWalkRoute(); break;
        case 4: _t->retryFollowWalkRoute(); break;
        case 5: _t->waitForPublicTransport(); break;
        case 6: _t->departingPublicTransport((*reinterpret_cast< PublicTransportVehicleDriver*(*)>(_a[1])),(*reinterpret_cast< PublicTransportVehicle*(*)>(_a[2])),(*reinterpret_cast< QList<PublicTransportStop*>(*)>(_a[3]))); break;
        case 7: _t->reachedStop((*reinterpret_cast< PublicTransportVehicleDriver*(*)>(_a[1])),(*reinterpret_cast< PublicTransportVehicle*(*)>(_a[2])),(*reinterpret_cast< PublicTransportStop*(*)>(_a[3]))); break;
        case 8: _t->routeFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PublicTransportVehicle* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PublicTransportVehicleDriver* >(); break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<PublicTransportStop*> >(); break;
            }
            break;
        case 7:
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
    }
}

const QMetaObject PublicTransportUserPerson::staticMetaObject = {
    { &Person::staticMetaObject, qt_meta_stringdata_PublicTransportUserPerson.data,
      qt_meta_data_PublicTransportUserPerson,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PublicTransportUserPerson::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PublicTransportUserPerson::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PublicTransportUserPerson.stringdata0))
        return static_cast<void*>(const_cast< PublicTransportUserPerson*>(this));
    return Person::qt_metacast(_clname);
}

int PublicTransportUserPerson::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Person::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

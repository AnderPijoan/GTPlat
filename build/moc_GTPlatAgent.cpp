/****************************************************************************
** Meta object code from reading C++ file 'GTPlatAgent.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "usecases/gtplat/GTPlatAgent.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GTPlatAgent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GTPlatAgent_t {
    QByteArrayData data[25];
    char stringdata0[393];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GTPlatAgent_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GTPlatAgent_t qt_meta_stringdata_GTPlatAgent = {
    {
QT_MOC_LITERAL(0, 0, 11), // "GTPlatAgent"
QT_MOC_LITERAL(1, 12, 6), // "behave"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 11), // "setLegRoute"
QT_MOC_LITERAL(4, 32, 15), // "followWalkRoute"
QT_MOC_LITERAL(5, 48, 14), // "walkRouteEnded"
QT_MOC_LITERAL(6, 63, 16), // "followDriveRoute"
QT_MOC_LITERAL(7, 80, 15), // "driveRouteEnded"
QT_MOC_LITERAL(8, 96, 22), // "waitForPublicTransport"
QT_MOC_LITERAL(9, 119, 24), // "departingPublicTransport"
QT_MOC_LITERAL(10, 144, 29), // "PublicTransportVehicleDriver*"
QT_MOC_LITERAL(11, 174, 6), // "driver"
QT_MOC_LITERAL(12, 181, 23), // "PublicTransportVehicle*"
QT_MOC_LITERAL(13, 205, 7), // "vehicle"
QT_MOC_LITERAL(14, 213, 27), // "QList<PublicTransportStop*>"
QT_MOC_LITERAL(15, 241, 15), // "remaining_stops"
QT_MOC_LITERAL(16, 257, 11), // "reachedStop"
QT_MOC_LITERAL(17, 269, 20), // "PublicTransportStop*"
QT_MOC_LITERAL(18, 290, 12), // "reached_stop"
QT_MOC_LITERAL(19, 303, 13), // "routeFinished"
QT_MOC_LITERAL(20, 317, 18), // "requestItineraries"
QT_MOC_LITERAL(21, 336, 18), // "waitForItineraries"
QT_MOC_LITERAL(22, 355, 18), // "choseBestItinerary"
QT_MOC_LITERAL(23, 374, 8), // "printLog"
QT_MOC_LITERAL(24, 383, 9) // "file_name"

    },
    "GTPlatAgent\0behave\0\0setLegRoute\0"
    "followWalkRoute\0walkRouteEnded\0"
    "followDriveRoute\0driveRouteEnded\0"
    "waitForPublicTransport\0departingPublicTransport\0"
    "PublicTransportVehicleDriver*\0driver\0"
    "PublicTransportVehicle*\0vehicle\0"
    "QList<PublicTransportStop*>\0remaining_stops\0"
    "reachedStop\0PublicTransportStop*\0"
    "reached_stop\0routeFinished\0"
    "requestItineraries\0waitForItineraries\0"
    "choseBestItinerary\0printLog\0file_name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GTPlatAgent[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x09 /* Protected */,
       4,    0,   86,    2, 0x09 /* Protected */,
       5,    0,   87,    2, 0x09 /* Protected */,
       6,    0,   88,    2, 0x09 /* Protected */,
       7,    0,   89,    2, 0x09 /* Protected */,
       8,    0,   90,    2, 0x09 /* Protected */,
       9,    3,   91,    2, 0x09 /* Protected */,
      16,    3,   98,    2, 0x09 /* Protected */,
      19,    0,  105,    2, 0x09 /* Protected */,
      20,    0,  106,    2, 0x09 /* Protected */,
      21,    0,  107,    2, 0x09 /* Protected */,
      22,    0,  108,    2, 0x09 /* Protected */,
      23,    1,  109,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 12, 0x80000000 | 14,   11,   13,   15,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 12, 0x80000000 | 17,   11,   13,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,

       0        // eod
};

void GTPlatAgent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GTPlatAgent *_t = static_cast<GTPlatAgent *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->behave(); break;
        case 1: _t->setLegRoute(); break;
        case 2: _t->followWalkRoute(); break;
        case 3: _t->walkRouteEnded(); break;
        case 4: _t->followDriveRoute(); break;
        case 5: _t->driveRouteEnded(); break;
        case 6: _t->waitForPublicTransport(); break;
        case 7: _t->departingPublicTransport((*reinterpret_cast< PublicTransportVehicleDriver*(*)>(_a[1])),(*reinterpret_cast< PublicTransportVehicle*(*)>(_a[2])),(*reinterpret_cast< QList<PublicTransportStop*>(*)>(_a[3]))); break;
        case 8: _t->reachedStop((*reinterpret_cast< PublicTransportVehicleDriver*(*)>(_a[1])),(*reinterpret_cast< PublicTransportVehicle*(*)>(_a[2])),(*reinterpret_cast< PublicTransportStop*(*)>(_a[3]))); break;
        case 9: _t->routeFinished(); break;
        case 10: _t->requestItineraries(); break;
        case 11: _t->waitForItineraries(); break;
        case 12: _t->choseBestItinerary(); break;
        case 13: _t->printLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<PublicTransportStop*> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PublicTransportStop* >(); break;
            }
            break;
        }
    }
}

const QMetaObject GTPlatAgent::staticMetaObject = {
    { &Person::staticMetaObject, qt_meta_stringdata_GTPlatAgent.data,
      qt_meta_data_GTPlatAgent,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GTPlatAgent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GTPlatAgent::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GTPlatAgent.stringdata0))
        return static_cast<void*>(const_cast< GTPlatAgent*>(this));
    return Person::qt_metacast(_clname);
}

int GTPlatAgent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Person::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'RoadConstants.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/transport_networks/roads/RoadConstants.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RoadConstants.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RoadConstants_t {
    QByteArrayData data[27];
    char stringdata0[304];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RoadConstants_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RoadConstants_t qt_meta_stringdata_RoadConstants = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RoadConstants"
QT_MOC_LITERAL(1, 14, 10), // "road_types"
QT_MOC_LITERAL(2, 25, 12), // "UNCLASSIFIED"
QT_MOC_LITERAL(3, 38, 8), // "MOTORWAY"
QT_MOC_LITERAL(4, 47, 13), // "MOTORWAY_LINK"
QT_MOC_LITERAL(5, 61, 17), // "MOTORWAY_JUNCTION"
QT_MOC_LITERAL(6, 79, 5), // "TRUNK"
QT_MOC_LITERAL(7, 85, 10), // "TRUNK_LINK"
QT_MOC_LITERAL(8, 96, 7), // "PRIMARY"
QT_MOC_LITERAL(9, 104, 12), // "PRIMARY_LINK"
QT_MOC_LITERAL(10, 117, 9), // "SECONDARY"
QT_MOC_LITERAL(11, 127, 14), // "SECONDARY_LINK"
QT_MOC_LITERAL(12, 142, 8), // "TERTIARY"
QT_MOC_LITERAL(13, 151, 13), // "TERTIARY_LINK"
QT_MOC_LITERAL(14, 165, 11), // "RESIDENTIAL"
QT_MOC_LITERAL(15, 177, 13), // "LIVING_STREET"
QT_MOC_LITERAL(16, 191, 7), // "SERVICE"
QT_MOC_LITERAL(17, 199, 5), // "TRACK"
QT_MOC_LITERAL(18, 205, 4), // "PATH"
QT_MOC_LITERAL(19, 210, 8), // "CYCLEWAY"
QT_MOC_LITERAL(20, 219, 9), // "BRIDLEWAY"
QT_MOC_LITERAL(21, 229, 4), // "ROAD"
QT_MOC_LITERAL(22, 234, 10), // "ROUNDABOUT"
QT_MOC_LITERAL(23, 245, 10), // "CYCLE_LANE"
QT_MOC_LITERAL(24, 256, 11), // "CYCLE_TRACK"
QT_MOC_LITERAL(25, 268, 20), // "CYCLE_OPPOSITE_TRACK"
QT_MOC_LITERAL(26, 289, 14) // "CYCLE_OPPOSITE"

    },
    "RoadConstants\0road_types\0UNCLASSIFIED\0"
    "MOTORWAY\0MOTORWAY_LINK\0MOTORWAY_JUNCTION\0"
    "TRUNK\0TRUNK_LINK\0PRIMARY\0PRIMARY_LINK\0"
    "SECONDARY\0SECONDARY_LINK\0TERTIARY\0"
    "TERTIARY_LINK\0RESIDENTIAL\0LIVING_STREET\0"
    "SERVICE\0TRACK\0PATH\0CYCLEWAY\0BRIDLEWAY\0"
    "ROAD\0ROUNDABOUT\0CYCLE_LANE\0CYCLE_TRACK\0"
    "CYCLE_OPPOSITE_TRACK\0CYCLE_OPPOSITE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RoadConstants[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,   25,   18,

 // enum data: key, value
       2, uint(RoadConstants::UNCLASSIFIED),
       3, uint(RoadConstants::MOTORWAY),
       4, uint(RoadConstants::MOTORWAY_LINK),
       5, uint(RoadConstants::MOTORWAY_JUNCTION),
       6, uint(RoadConstants::TRUNK),
       7, uint(RoadConstants::TRUNK_LINK),
       8, uint(RoadConstants::PRIMARY),
       9, uint(RoadConstants::PRIMARY_LINK),
      10, uint(RoadConstants::SECONDARY),
      11, uint(RoadConstants::SECONDARY_LINK),
      12, uint(RoadConstants::TERTIARY),
      13, uint(RoadConstants::TERTIARY_LINK),
      14, uint(RoadConstants::RESIDENTIAL),
      15, uint(RoadConstants::LIVING_STREET),
      16, uint(RoadConstants::SERVICE),
      17, uint(RoadConstants::TRACK),
      18, uint(RoadConstants::PATH),
      19, uint(RoadConstants::CYCLEWAY),
      20, uint(RoadConstants::BRIDLEWAY),
      21, uint(RoadConstants::ROAD),
      22, uint(RoadConstants::ROUNDABOUT),
      23, uint(RoadConstants::CYCLE_LANE),
      24, uint(RoadConstants::CYCLE_TRACK),
      25, uint(RoadConstants::CYCLE_OPPOSITE_TRACK),
      26, uint(RoadConstants::CYCLE_OPPOSITE),

       0        // eod
};

void RoadConstants::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject RoadConstants::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RoadConstants.data,
      qt_meta_data_RoadConstants,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RoadConstants::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RoadConstants::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RoadConstants.stringdata0))
        return static_cast<void*>(const_cast< RoadConstants*>(this));
    return QObject::qt_metacast(_clname);
}

int RoadConstants::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

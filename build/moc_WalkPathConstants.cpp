/****************************************************************************
** Meta object code from reading C++ file 'WalkPathConstants.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/transport_networks/walk_paths/WalkPathConstants.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WalkPathConstants.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WalkPathConstants_t {
    QByteArrayData data[19];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WalkPathConstants_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WalkPathConstants_t qt_meta_stringdata_WalkPathConstants = {
    {
QT_MOC_LITERAL(0, 0, 17), // "WalkPathConstants"
QT_MOC_LITERAL(1, 18, 15), // "walk_path_types"
QT_MOC_LITERAL(2, 34, 12), // "UNCLASSIFIED"
QT_MOC_LITERAL(3, 47, 7), // "PRIMARY"
QT_MOC_LITERAL(4, 55, 12), // "PRIMARY_LINK"
QT_MOC_LITERAL(5, 68, 9), // "SECONDARY"
QT_MOC_LITERAL(6, 78, 14), // "SECONDARY_LINK"
QT_MOC_LITERAL(7, 93, 8), // "TERTIARY"
QT_MOC_LITERAL(8, 102, 13), // "TERTIARY_LINK"
QT_MOC_LITERAL(9, 116, 11), // "RESIDENTIAL"
QT_MOC_LITERAL(10, 128, 13), // "LIVING_STREET"
QT_MOC_LITERAL(11, 142, 7), // "SERVICE"
QT_MOC_LITERAL(12, 150, 5), // "TRACK"
QT_MOC_LITERAL(13, 156, 4), // "PATH"
QT_MOC_LITERAL(14, 161, 10), // "PEDESTRIAN"
QT_MOC_LITERAL(15, 172, 7), // "FOOTWAY"
QT_MOC_LITERAL(16, 180, 5), // "STEPS"
QT_MOC_LITERAL(17, 186, 4), // "ROAD"
QT_MOC_LITERAL(18, 191, 10) // "ROUNDABOUT"

    },
    "WalkPathConstants\0walk_path_types\0"
    "UNCLASSIFIED\0PRIMARY\0PRIMARY_LINK\0"
    "SECONDARY\0SECONDARY_LINK\0TERTIARY\0"
    "TERTIARY_LINK\0RESIDENTIAL\0LIVING_STREET\0"
    "SERVICE\0TRACK\0PATH\0PEDESTRIAN\0FOOTWAY\0"
    "STEPS\0ROAD\0ROUNDABOUT"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WalkPathConstants[] = {

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
       1, 0x0,   17,   18,

 // enum data: key, value
       2, uint(WalkPathConstants::UNCLASSIFIED),
       3, uint(WalkPathConstants::PRIMARY),
       4, uint(WalkPathConstants::PRIMARY_LINK),
       5, uint(WalkPathConstants::SECONDARY),
       6, uint(WalkPathConstants::SECONDARY_LINK),
       7, uint(WalkPathConstants::TERTIARY),
       8, uint(WalkPathConstants::TERTIARY_LINK),
       9, uint(WalkPathConstants::RESIDENTIAL),
      10, uint(WalkPathConstants::LIVING_STREET),
      11, uint(WalkPathConstants::SERVICE),
      12, uint(WalkPathConstants::TRACK),
      13, uint(WalkPathConstants::PATH),
      14, uint(WalkPathConstants::PEDESTRIAN),
      15, uint(WalkPathConstants::FOOTWAY),
      16, uint(WalkPathConstants::STEPS),
      17, uint(WalkPathConstants::ROAD),
      18, uint(WalkPathConstants::ROUNDABOUT),

       0        // eod
};

void WalkPathConstants::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject WalkPathConstants::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WalkPathConstants.data,
      qt_meta_data_WalkPathConstants,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WalkPathConstants::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WalkPathConstants::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WalkPathConstants.stringdata0))
        return static_cast<void*>(const_cast< WalkPathConstants*>(this));
    return QObject::qt_metacast(_clname);
}

int WalkPathConstants::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

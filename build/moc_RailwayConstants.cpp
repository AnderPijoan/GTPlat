/****************************************************************************
** Meta object code from reading C++ file 'RailwayConstants.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/transport_networks/railways/RailwayConstants.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RailwayConstants.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RailwayConstants_t {
    QByteArrayData data[10];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RailwayConstants_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RailwayConstants_t qt_meta_stringdata_RailwayConstants = {
    {
QT_MOC_LITERAL(0, 0, 16), // "RailwayConstants"
QT_MOC_LITERAL(1, 17, 13), // "railway_types"
QT_MOC_LITERAL(2, 31, 12), // "UNCLASSIFIED"
QT_MOC_LITERAL(3, 44, 9), // "FUNICULAR"
QT_MOC_LITERAL(4, 54, 10), // "LIGHT_RAIL"
QT_MOC_LITERAL(5, 65, 12), // "NARROW_GAUGE"
QT_MOC_LITERAL(6, 78, 9), // "PRESERVED"
QT_MOC_LITERAL(7, 88, 4), // "RAIL"
QT_MOC_LITERAL(8, 93, 6), // "SUBWAY"
QT_MOC_LITERAL(9, 100, 4) // "TRAM"

    },
    "RailwayConstants\0railway_types\0"
    "UNCLASSIFIED\0FUNICULAR\0LIGHT_RAIL\0"
    "NARROW_GAUGE\0PRESERVED\0RAIL\0SUBWAY\0"
    "TRAM"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RailwayConstants[] = {

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
       1, 0x0,    8,   18,

 // enum data: key, value
       2, uint(RailwayConstants::UNCLASSIFIED),
       3, uint(RailwayConstants::FUNICULAR),
       4, uint(RailwayConstants::LIGHT_RAIL),
       5, uint(RailwayConstants::NARROW_GAUGE),
       6, uint(RailwayConstants::PRESERVED),
       7, uint(RailwayConstants::RAIL),
       8, uint(RailwayConstants::SUBWAY),
       9, uint(RailwayConstants::TRAM),

       0        // eod
};

void RailwayConstants::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject RailwayConstants::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RailwayConstants.data,
      qt_meta_data_RailwayConstants,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RailwayConstants::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RailwayConstants::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RailwayConstants.stringdata0))
        return static_cast<void*>(const_cast< RailwayConstants*>(this));
    return QObject::qt_metacast(_clname);
}

int RailwayConstants::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

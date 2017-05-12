/****************************************************************************
** Meta object code from reading C++ file 'PollutionConstants.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "skills/produce/pollution/PollutionConstants.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PollutionConstants.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PollutionConstants_t {
    QByteArrayData data[16];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PollutionConstants_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PollutionConstants_t qt_meta_stringdata_PollutionConstants = {
    {
QT_MOC_LITERAL(0, 0, 18), // "PollutionConstants"
QT_MOC_LITERAL(1, 19, 15), // "pollutant_types"
QT_MOC_LITERAL(2, 35, 2), // "CC"
QT_MOC_LITERAL(3, 38, 3), // "CH4"
QT_MOC_LITERAL(4, 42, 2), // "CO"
QT_MOC_LITERAL(5, 45, 3), // "CO2"
QT_MOC_LITERAL(6, 49, 4), // "DALY"
QT_MOC_LITERAL(7, 54, 2), // "FC"
QT_MOC_LITERAL(8, 57, 2), // "HC"
QT_MOC_LITERAL(9, 60, 3), // "NH3"
QT_MOC_LITERAL(10, 64, 3), // "NOx"
QT_MOC_LITERAL(11, 68, 3), // "N2O"
QT_MOC_LITERAL(12, 72, 2), // "PM"
QT_MOC_LITERAL(13, 75, 2), // "PN"
QT_MOC_LITERAL(14, 78, 3), // "SO2"
QT_MOC_LITERAL(15, 82, 2) // "WH"

    },
    "PollutionConstants\0pollutant_types\0"
    "CC\0CH4\0CO\0CO2\0DALY\0FC\0HC\0NH3\0NOx\0N2O\0"
    "PM\0PN\0SO2\0WH"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PollutionConstants[] = {

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
       1, 0x0,   14,   18,

 // enum data: key, value
       2, uint(PollutionConstants::CC),
       3, uint(PollutionConstants::CH4),
       4, uint(PollutionConstants::CO),
       5, uint(PollutionConstants::CO2),
       6, uint(PollutionConstants::DALY),
       7, uint(PollutionConstants::FC),
       8, uint(PollutionConstants::HC),
       9, uint(PollutionConstants::NH3),
      10, uint(PollutionConstants::NOx),
      11, uint(PollutionConstants::N2O),
      12, uint(PollutionConstants::PM),
      13, uint(PollutionConstants::PN),
      14, uint(PollutionConstants::SO2),
      15, uint(PollutionConstants::WH),

       0        // eod
};

void PollutionConstants::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PollutionConstants::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PollutionConstants.data,
      qt_meta_data_PollutionConstants,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PollutionConstants::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PollutionConstants::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PollutionConstants.stringdata0))
        return static_cast<void*>(const_cast< PollutionConstants*>(this));
    return QObject::qt_metacast(_clname);
}

int PollutionConstants::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

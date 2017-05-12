/****************************************************************************
** Meta object code from reading C++ file 'PublicTransportStretch.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "agents/transport_networks/public_transport/PublicTransportStretch.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PublicTransportStretch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PublicTransportStretch_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PublicTransportStretch_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PublicTransportStretch_t qt_meta_stringdata_PublicTransportStretch = {
    {
QT_MOC_LITERAL(0, 0, 22) // "PublicTransportStretch"

    },
    "PublicTransportStretch"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PublicTransportStretch[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void PublicTransportStretch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PublicTransportStretch::staticMetaObject = {
    { &Agent::staticMetaObject, qt_meta_stringdata_PublicTransportStretch.data,
      qt_meta_data_PublicTransportStretch,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PublicTransportStretch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PublicTransportStretch::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PublicTransportStretch.stringdata0))
        return static_cast<void*>(const_cast< PublicTransportStretch*>(this));
    if (!strcmp(_clname, "GSSGraphEdge"))
        return static_cast< GSSGraphEdge*>(const_cast< PublicTransportStretch*>(this));
    return Agent::qt_metacast(_clname);
}

int PublicTransportStretch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Agent::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

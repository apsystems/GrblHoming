/****************************************************************************
** Meta object code from reading C++ file 'readthread.h'
**
** Created: Tue Mar 20 13:24:11 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "readthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'readthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReadThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,
      38,   34,   11,   11, 0x05,
      60,   55,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ReadThread[] = {
    "ReadThread\0\0line\0addList(QString)\0msg\0"
    "sendMsg(QString)\0axis\0sendAxis(QString)\0"
    "stopsig()\0"
};

void ReadThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ReadThread *_t = static_cast<ReadThread *>(_o);
        switch (_id) {
        case 0: _t->addList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sendMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendAxis((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->stopsig(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ReadThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ReadThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ReadThread,
      qt_meta_data_ReadThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ReadThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ReadThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ReadThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReadThread))
        return static_cast<void*>(const_cast< ReadThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ReadThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ReadThread::addList(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ReadThread::sendMsg(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ReadThread::sendAxis(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

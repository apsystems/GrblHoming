/****************************************************************************
** Meta object code from reading C++ file 'grbldialog.h'
**
** Created: Tue Mar 20 13:24:15 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "grbldialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'grbldialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GrblDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      17,   11,   11,   11, 0x0a,
      34,   26,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GrblDialog[] = {
    "GrblDialog\0\0Ok()\0Cancel()\0row,col\0"
    "changeValues(int,int)\0"
};

void GrblDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GrblDialog *_t = static_cast<GrblDialog *>(_o);
        switch (_id) {
        case 0: _t->Ok(); break;
        case 1: _t->Cancel(); break;
        case 2: _t->changeValues((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GrblDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GrblDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GrblDialog,
      qt_meta_data_GrblDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GrblDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GrblDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GrblDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GrblDialog))
        return static_cast<void*>(const_cast< GrblDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int GrblDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

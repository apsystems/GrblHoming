/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon 20. Feb 15:50:24 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      19,   11,   11,   11, 0x08,
      26,   11,   11,   11, 0x08,
      33,   11,   11,   11, 0x08,
      40,   11,   11,   11, 0x08,
      47,   11,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,
      61,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
      83,   11,   11,   11, 0x08,
      90,   11,   11,   11, 0x08,
      98,   11,   11,   11, 0x08,
     109,  105,   11,   11, 0x08,
     130,  105,   11,   11, 0x08,
     155,  150,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0Stop()\0incX()\0incY()\0"
    "incZ()\0decX()\0decY()\0decZ()\0adjustBtn()\0"
    "sendBtn()\0open()\0begin()\0stop()\0msg\0"
    "receiveList(QString)\0receiveMsg(QString)\0"
    "axis\0receiveAxis(QString)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->Stop(); break;
        case 1: _t->incX(); break;
        case 2: _t->incY(); break;
        case 3: _t->incZ(); break;
        case 4: _t->decX(); break;
        case 5: _t->decY(); break;
        case 6: _t->decZ(); break;
        case 7: _t->adjustBtn(); break;
        case 8: _t->sendBtn(); break;
        case 9: _t->open(); break;
        case 10: _t->begin(); break;
        case 11: _t->stop(); break;
        case 12: _t->receiveList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->receiveMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->receiveAxis((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::Stop()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

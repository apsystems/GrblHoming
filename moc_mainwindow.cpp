/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue Mar 20 13:24:08 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
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
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      19,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      40,   11,   11,   11, 0x08,
      47,   11,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,
      61,   11,   11,   11, 0x08,
      68,   11,   11,   11, 0x08,
      75,   11,   11,   11, 0x08,
      82,   11,   11,   11, 0x08,
      90,   11,   11,   11, 0x08,
     101,   11,   11,   11, 0x08,
     118,   11,   11,   11, 0x08,
     128,   11,   11,   11, 0x08,
     136,   11,   11,   11, 0x08,
     147,   11,   11,   11, 0x08,
     154,   11,   11,   11, 0x08,
     167,   11,   11,   11, 0x08,
     180,   11,   11,   11, 0x08,
     200,  191,   11,   11, 0x08,
     215,   11,   11,   11, 0x08,
     240,  231,   11,   11, 0x08,
     264,  257,   11,   11, 0x08,
     288,  284,   11,   11, 0x08,
     309,  284,   11,   11, 0x08,
     334,  329,   11,   11, 0x08,
     355,   11,   11,   11, 0x08,
     368,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0Stop()\0openPort()\0setGRBL()\0"
    "decX()\0decY()\0decZ()\0incX()\0incY()\0"
    "incZ()\0reset()\0gotoHome()\0gotoToolChange()\0"
    "gotoXYZ()\0begin()\0openFile()\0stop()\0"
    "adjustRBtn()\0manualRBtn()\0sendRBtn()\0"
    "selected\0selectFav(int)\0toggleSpindle()\0"
    "settings\0setSettings(int)\0coords\0"
    "setTCCoord(float[])\0msg\0receiveList(QString)\0"
    "receiveMsg(QString)\0axis\0receiveAxis(QString)\0"
    "getOptions()\0showAbout()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->Stop(); break;
        case 1: _t->openPort(); break;
        case 2: _t->setGRBL(); break;
        case 3: _t->decX(); break;
        case 4: _t->decY(); break;
        case 5: _t->decZ(); break;
        case 6: _t->incX(); break;
        case 7: _t->incY(); break;
        case 8: _t->incZ(); break;
        case 9: _t->reset(); break;
        case 10: _t->gotoHome(); break;
        case 11: _t->gotoToolChange(); break;
        case 12: _t->gotoXYZ(); break;
        case 13: _t->begin(); break;
        case 14: _t->openFile(); break;
        case 15: _t->stop(); break;
        case 16: _t->adjustRBtn(); break;
        case 17: _t->manualRBtn(); break;
        case 18: _t->sendRBtn(); break;
        case 19: _t->selectFav((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->toggleSpindle(); break;
        case 21: _t->setSettings((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->setTCCoord((*reinterpret_cast< float(*)[]>(_a[1]))); break;
        case 23: _t->receiveList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->receiveMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->receiveAxis((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->getOptions(); break;
        case 27: _t->showAbout(); break;
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
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::Stop()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

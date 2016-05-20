/****************************************************************************
** Meta object code from reading C++ file 'LaserCommanderMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "LaserCommanderMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LaserCommanderMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LaserCommanderMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   26,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   25,   25,   25, 0x0a,
      56,   25,   25,   25, 0x09,
      70,   25,   25,   25, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_LaserCommanderMainWindow[] = {
    "LaserCommanderMainWindow\0\0pvalue\0"
    "changed(double)\0quit()\0testManager()\0"
    "updateD()\0"
};

void LaserCommanderMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LaserCommanderMainWindow *_t = static_cast<LaserCommanderMainWindow *>(_o);
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->quit(); break;
        case 2: _t->testManager(); break;
        case 3: _t->updateD(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LaserCommanderMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LaserCommanderMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_LaserCommanderMainWindow,
      qt_meta_data_LaserCommanderMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LaserCommanderMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LaserCommanderMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LaserCommanderMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LaserCommanderMainWindow))
        return static_cast<void*>(const_cast< LaserCommanderMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int LaserCommanderMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
void LaserCommanderMainWindow::changed(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

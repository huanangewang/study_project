/****************************************************************************
** Meta object code from reading C++ file 'MsgWebView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../MsgWebView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MsgWebView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MsgHtmlObj_t {
    QByteArrayData data[6];
    char stringdata0[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MsgHtmlObj_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MsgHtmlObj_t qt_meta_stringdata_MsgHtmlObj = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MsgHtmlObj"
QT_MOC_LITERAL(1, 11, 13), // "signalMsgHtml"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "html"
QT_MOC_LITERAL(4, 31, 12), // "msgLHtmlTmpl"
QT_MOC_LITERAL(5, 44, 12) // "msgRHtmlTmpl"

    },
    "MsgHtmlObj\0signalMsgHtml\0\0html\0"
    "msgLHtmlTmpl\0msgRHtmlTmpl"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MsgHtmlObj[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   22, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // properties: name, type, flags
       4, QMetaType::QString, 0x00495003,
       5, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

void MsgHtmlObj::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MsgHtmlObj *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalMsgHtml((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MsgHtmlObj::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MsgHtmlObj::signalMsgHtml)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<MsgHtmlObj *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->m_msgLHtmlTmpl; break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->m_msgRHtmlTmpl; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<MsgHtmlObj *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_msgLHtmlTmpl != *reinterpret_cast< QString*>(_v)) {
                _t->m_msgLHtmlTmpl = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->signalMsgHtml(_t->m_msgLHtmlTmpl);
            }
            break;
        case 1:
            if (_t->m_msgRHtmlTmpl != *reinterpret_cast< QString*>(_v)) {
                _t->m_msgRHtmlTmpl = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->signalMsgHtml(_t->m_msgRHtmlTmpl);
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject MsgHtmlObj::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MsgHtmlObj.data,
    qt_meta_data_MsgHtmlObj,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MsgHtmlObj::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MsgHtmlObj::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MsgHtmlObj.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MsgHtmlObj::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MsgHtmlObj::signalMsgHtml(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_MsgWebPage_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MsgWebPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MsgWebPage_t qt_meta_stringdata_MsgWebPage = {
    {
QT_MOC_LITERAL(0, 0, 10) // "MsgWebPage"

    },
    "MsgWebPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MsgWebPage[] = {

 // content:
       8,       // revision
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

void MsgWebPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MsgWebPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWebEnginePage::staticMetaObject>(),
    qt_meta_stringdata_MsgWebPage.data,
    qt_meta_data_MsgWebPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MsgWebPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MsgWebPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MsgWebPage.stringdata0))
        return static_cast<void*>(this);
    return QWebEnginePage::qt_metacast(_clname);
}

int MsgWebPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEnginePage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_MsgWebView_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MsgWebView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MsgWebView_t qt_meta_stringdata_MsgWebView = {
    {
QT_MOC_LITERAL(0, 0, 10) // "MsgWebView"

    },
    "MsgWebView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MsgWebView[] = {

 // content:
       8,       // revision
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

void MsgWebView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MsgWebView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWebEngineView::staticMetaObject>(),
    qt_meta_stringdata_MsgWebView.data,
    qt_meta_data_MsgWebView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MsgWebView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MsgWebView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MsgWebView.stringdata0))
        return static_cast<void*>(this);
    return QWebEngineView::qt_metacast(_clname);
}

int MsgWebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEngineView::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

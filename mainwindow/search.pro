#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T01:46:53
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    db.cpp \
    keymanager.cpp \
    db_manager.cpp


HEADERS  += mainwindow.h \
    db.h \
    keymanager.h \
    db_manager.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../friso/release/ -lfriso
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../friso/debug/ -lfriso
else:unix: LIBS += -L$$PWD/../friso/ -lfriso

INCLUDEPATH += $$PWD/../friso
DEPENDPATH += $$PWD/../friso

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../friso/release/libfriso.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../friso/debug/libfriso.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../friso/release/friso.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../friso/debug/friso.lib
else:unix: PRE_TARGETDEPS += $$PWD/../friso/libfriso.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../sm/release/ -lsm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../sm/debug/ -lsm
else:unix: LIBS += -L$$PWD/../sm/ -lsm

INCLUDEPATH += $$PWD/../sm
DEPENDPATH += $$PWD/../sm

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../sm/release/libsm.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../sm/debug/libsm.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../sm/release/sm.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../sm/debug/sm.lib
else:unix: PRE_TARGETDEPS += $$PWD/../sm/libsm.a

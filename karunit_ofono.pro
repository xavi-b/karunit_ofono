TEMPLATE        = lib
CONFIG         += plugin c++17
DEFINES        += QT_DEPRECATED_WARNINGS
QT             += quick dbus
TARGET          = karunit_ofono_plugin
DESTDIR         = $$PWD/../karunit/app/plugins

unix {
target.path = /usr/local/bin/plugins
INSTALLS += target
}

libqofono.target = $$PWD/third-party/libqofono/src/libqofono-qt5.so
libqofono.commands += cd $$PWD/third-party/libqofono && qmake && make -j4
QMAKE_EXTRA_TARGETS += libqofono
PRE_TARGETDEPS += $$libqofono.target

libqofono.files = $$PWD/third-party/libqofono/src/libqofono-qt5.so*
libqofono.path = /usr/lib
INSTALLS += libqofono

LIBS += -L$$PWD/third-party/libqofono/src/ -lqofono-qt5
INCLUDEPATH += $$PWD/third-party/libqofono/src/

LIBS += -L$$PWD/../karunit/plugininterface/ -lkarunit_plugininterface
INCLUDEPATH += $$PWD/../karunit/plugininterface

LIBS += -L$$PWD/../karunit/common/ -lkarunit_common
INCLUDEPATH += $$PWD/../karunit/common

LIBS += -L$$PWD/../karunit/third-party/xblog/lib -lxblog
INCLUDEPATH += $$PWD/../karunit/third-party/xblog/include

SUBDIRS += \
    src/ \
    res/

include(src/src.pri)
include(res/res.pri)

TEMPLATE        = lib
CONFIG         += plugin c++17
DEFINES        += QT_DEPRECATED_WARNINGS
QT             += widgets dbus
TARGET          = karunit_ofono_plugin
DESTDIR         = $$PWD/../karunit/app/plugins

LIBS += -L$$PWD/third-party/libqofono/src/ -lqofono-qt5
INCLUDEPATH += $$PWD/third-party/libqofono/src/

LIBS += -L$$PWD/../karunit/plugininterface/ -lplugininterface
INCLUDEPATH += $$PWD/../karunit/plugininterface

LIBS += -L$$PWD/../karunit/common/ -lcommon
INCLUDEPATH += $$PWD/../karunit/common

LIBS += -L$$PWD/../karunit/third-party/xblog/ -lxblog
INCLUDEPATH += $$PWD/../karunit/third-party/xblog/src

SUBDIRS += \
    src/

include(src/src.pri)

RESOURCES += \
    karunit_ofono.qrc

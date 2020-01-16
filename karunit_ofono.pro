TEMPLATE        = lib
CONFIG         += plugin c++17
DEFINES        += QT_DEPRECATED_WARNINGS
QT             += widgets dbus
TARGET          = karunit_ofono_plugin
DESTDIR         = $$PWD/../karunit/app/plugins

LIBS += -lqofono-qt5

LIBS += -L$$PWD/../karunit/lib/
LIBS += -L$$PWD/../karunit/app/plugins/

LIBS += -lplugininterface
INCLUDEPATH += $$PWD/../karunit/plugininterface
DEPENDPATH += $$PWD/../karunit/plugininterface

LIBS += -lcommon
INCLUDEPATH += $$PWD/../karunit/common
DEPENDPATH += $$PWD/../karunit/common

SUBDIRS += \
    src/

include(src/src.pri)

RESOURCES += \
    karunit_ofono.qrc

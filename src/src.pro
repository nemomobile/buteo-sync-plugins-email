TARGET = syncemail-client

CONFIG += link_pkgconfig
PKGCONFIG += buteosyncfw5 qmfclient5 nemoemail-qt5 accounts-qt5

QT += network dbus
QT -= gui


!contains (DEFINES, OUT_OF_PROCESS_PLUGIN) {
    TEMPLATE = lib
    CONFIG += plugin
    target.path = /usr/lib/buteo-plugins-qt5
    message("building" $$TARGET "as in-process plugin")
}
contains (DEFINES, OUT_OF_PROCESS_PLUGIN) {
    TEMPLATE = app
    target.path = /usr/lib/buteo-plugins-qt5/oopp
    message("building" $$TARGET "as out-of-process plugin")

    DEFINES += CLIENT_PLUGIN
    BUTEO_OOPP_INCLUDE_DIR = $$system(pkg-config --cflags buteosyncfw5|cut -f2 -d'I')
    INCLUDEPATH += $$BUTEO_OOPP_INCLUDE_DIR

    HEADERS += $$BUTEO_OOPP_INCLUDE_DIR/ButeoPluginIfaceAdaptor.h   \
               $$BUTEO_OOPP_INCLUDE_DIR/PluginCbImpl.h              \
               $$BUTEO_OOPP_INCLUDE_DIR/PluginServiceObj.h

    SOURCES += $$BUTEO_OOPP_INCLUDE_DIR/ButeoPluginIfaceAdaptor.cpp \
               $$BUTEO_OOPP_INCLUDE_DIR/PluginCbImpl.cpp            \
               $$BUTEO_OOPP_INCLUDE_DIR/PluginServiceObj.cpp        \
               $$BUTEO_OOPP_INCLUDE_DIR/plugin_main.cpp

    DEFINES += "CLASSNAME=SyncEmailClient"
    DEFINES += CLASSNAME_H=\\\"syncemailclient.h\\\"

    # BUTEO_OOPP_QCONNMANBEARER_BUG_WORKAROUND requires -ldl
    DEFINES += BUTEO_OOPP_QCONNMANBEARER_BUG_WORKAROUND=qconnmanbearer_bug_workaround
    LIBS += -ldl
}

HEADERS += syncemailclient.h

SOURCES += syncemailclient.cpp

sync.path = /etc/buteo/profiles/sync
sync.files = ../xmls/sync/*

client.path = /etc/buteo/profiles/client
client.files = ../xmls/client/*

INSTALLS += target sync client

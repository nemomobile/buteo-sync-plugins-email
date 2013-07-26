TEMPLATE = lib
TARGET = syncemail-client

QT += network

CONFIG += link_pkgconfig plugin
PKGCONFIG += buteosyncfw5 qmfclient5 nemoemail-qt5


HEADERS += syncemailclient.h

SOURCES += syncemailclient.cpp

OTHER_FILES += xmls/sync/syncemail.xml \
    xmls/client/syncemail.xml \
    rpm/buteo-sync-plugins-email.spec

target.path = /usr/lib/buteo-plugins-qt5

sync.path = /etc/buteo/profiles/sync
sync.files = xmls/sync/*

client.path = /etc/buteo/profiles/client
client.files = xmls/client/*

INSTALLS += target sync client

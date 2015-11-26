TEMPLATE = app

QT += qml quick
CONFIG += c++11

SUBDIRS += src \
           3rdparty

SOURCES += main.cpp \
    consoleclient.cpp

RESOURCES += qml.qrc

INCLUDEPATH += $$PWD/../3rdparty/qxmpp/src/base \
               $$PWD/../3rdparty/qxmpp/src/client

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    consoleclient.h

LIBS += -L $$PWD/../3rdparty/qxmpp/src -lqxmpp

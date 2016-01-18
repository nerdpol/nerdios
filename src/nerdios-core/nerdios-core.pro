TEMPLATE = app

TARGET = nerdios

QT += qml quick widgets macextras
CONFIG += c++11

SUBDIRS += ../../3rdparty

SOURCES += main.cpp \
    consoleclient.cpp \
    nerdioscore.cpp \
    qxmppmessageqml.cpp \
    tcpremoteserver.cpp \
    contactlist.cpp

RESOURCES += qml.qrc

INCLUDEPATH += $$PWD/../../3rdparty/qxmpp/src/base \
               $$PWD/../../3rdparty/qxmpp/src/client

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    consoleclient.h \
    nerdioscore.h \
    qxmppmessageqml.h \
    tcpremoteserver.h \
    contactlist.h \
    utils.h

CONFIG(debug, debug|release) {
        LIBS += -L../../3rdparty/qxmpp/src -lqxmpp_d
} else {
        LIBS += -L../../3rdparty/qxmpp/src -lqxmpp
}

ICON = nerdios-core.icns

TEMPLATE = app

TARGET = nerdios-remote

CONFIG += console

CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

QT -= gui
QT += network

HEADERS += \
    nerdiosremote.h

SOURCES += \
    nerdiosremote.cpp \
    main.cpp

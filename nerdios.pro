TEMPLATE = subdirs

QT += qml quick
CONFIG += c++11

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

SUBDIRS += src \
           3rdparty/qxmpp

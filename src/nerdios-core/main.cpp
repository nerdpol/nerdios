#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QXmppClient.h>

#include "consoleclient.h"
#include "nerdioscore.h"
#include "qxmppmessageqml.h"
#include "tcpremoteserver.h"
#include "contactlist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("nerdpol Inc.");
    QGuiApplication::setOrganizationDomain("nerdpol.io");
    QGuiApplication::setApplicationName("nerdios");
    QGuiApplication::setApplicationVersion("0.1.0");
    QGuiApplication::setWindowIcon(QIcon(":/nerdios-core.icns"));

    QQmlApplicationEngine engine;
    // register types to use them in QML
    qmlRegisterUncreatableType<NerdiosCore>("nerdioscore", 1, 0, "NerdiosCore", "Do not create");
    qmlRegisterType<QXmppClient>("nerdioscore", 1, 0, "QXmppClient");
    qmlRegisterType<QXMPPMessageQML>("nerdioscore", 1, 0, "QXMPPMessageQML");
    qmlRegisterType<ContactList>("nerdioscore", 1, 0, "ContactList");

    // start NerdiosCore and connect a TCPRemoteServer instance
    NerdiosCore *nerdioscore = new NerdiosCore();
    TCPRemoteServer *tcpremoteserver = new TCPRemoteServer(nerdioscore);

    // load nerdioscore into QML
    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty("nerdioscore", nerdioscore);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

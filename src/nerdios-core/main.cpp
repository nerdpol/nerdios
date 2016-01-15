#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QXmppClient.h>

#include "consoleclient.h"
#include "nerdioscore.h"
#include "qxmppmessageqml.h"
#include "tcpremoteserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("nerdpol Inc.");
    QGuiApplication::setOrganizationDomain("nerdpol.io");
    QGuiApplication::setApplicationName("nerdios");

    QQmlApplicationEngine engine;
    qmlRegisterUncreatableType<NerdiosCore>("nerdioscore", 1, 0, "NerdiosCore", "Do not create");
    qmlRegisterType<QXmppClient>("nerdioscore", 1, 0, "QXmppClient");
    qmlRegisterType<QXMPPMessageQML>("nerdioscore", 1, 0, "QXMPPMessageQML");

    NerdiosCore *nerdioscore = new NerdiosCore();
    TCPRemoteServer *tcpremoteserver = new TCPRemoteServer(nerdioscore);

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty("nerdioscore", nerdioscore);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    /*
    QCoreApplication app(argc, argv);
    ConsoleClient client;
    client.prolog();
    */
    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QXmppClient.h>

#include "consoleclient.h"
#include "nerdioscore.h"
#include "qxmppmessageqml.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QGuiApplication::setOrganizationName("nerdpol Inc.");
    QGuiApplication::setOrganizationDomain("nerdpol.io");
    QGuiApplication::setApplicationName("nerdios");

    qmlRegisterType<NerdiosCore>("nerdioscore", 1, 0, "NerdiosCore");
    qmlRegisterType<QXmppClient>("qxmppclient", 1, 0, "QXmppClient");
    qmlRegisterType<QXMPPMessageQML>("qxmmpmessage", 1, 0, "QXMPPMessageQML");
    //qmlRegisterType<QXmppClient::State>("qxmppclientstate", 1, 0, "QXmppClientState");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    /*
    QCoreApplication app(argc, argv);
    ConsoleClient client;
    client.prolog();
    */
    return app.exec();
}

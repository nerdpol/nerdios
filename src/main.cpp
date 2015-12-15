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
    QGuiApplication::setOrganizationName("nerdpol Inc.");
    QGuiApplication::setOrganizationDomain("nerdpol.io");
    QGuiApplication::setApplicationName("nerdios");

    QQmlApplicationEngine engine;

    qmlRegisterType<NerdiosCore>("nerdioscore", 1, 0, "NerdiosCore");
    qmlRegisterType<QXmppClient>("nerdioscore", 1, 0, "QXmppClient");
    qmlRegisterType<QXMPPMessageQML>("nerdioscore", 1, 0, "QXMPPMessageQML");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    /*
    QCoreApplication app(argc, argv);
    ConsoleClient client;
    client.prolog();
    */
    return app.exec();
}

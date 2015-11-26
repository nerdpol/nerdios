#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "consoleclient.h"

int main(int argc, char *argv[])
{
    /*
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QGuiApplication::setOrganizationName("nerdpol Inc.");
    QGuiApplication::setOrganizationDomain("nerdpol.io");
    QGuiApplication::setApplicationName("nerdios");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    */

    QCoreApplication app(argc, argv);
    ConsoleClient client;
    client.prolog();
    return app.exec();
}

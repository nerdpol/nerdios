#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QStringList>

#include "nerdiosremote.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("nerdpol Inc.");
    QCoreApplication::setOrganizationDomain("nerdpol.io");
    QCoreApplication::setApplicationName("nerdios-remote");
    QCoreApplication::setApplicationVersion("0.1.0");

    QTextStream out(stdout);
    QTextStream err(stderr);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Remote cli for nerdios"));
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("command", QCoreApplication::translate("main", "Command to execute. Options: state, status, contacs, jid, priority"));
    QCommandLineOption hostOption("host", QCoreApplication::translate("main", "Specify a host (default is localhost)."), QCoreApplication::translate("main", "host"), "localhost");
    QCommandLineOption portOption("port", QCoreApplication::translate("main", "Specify a port (default is 1337)."), QCoreApplication::translate("main", "port"), "1337");
    parser.addOption(hostOption);
    parser.addOption(portOption);

    parser.process(app);
    const QStringList args = parser.positionalArguments();
    const QString host = parser.value(hostOption);
    const int port = parser.value(portOption).toInt();

    // Check arguments and options
    if (args.size() != 1) {
        err << QCoreApplication::translate("main", "Error: Must specify one command argument.") << endl;
        parser.showHelp(1);
    }

    if (port < 1 || port > 65536) {
        err << QCoreApplication::translate("main", "Error: Invalid port argument. Must be a valid port number (1-65536)") << endl;
        parser.showHelp(1);
    }

    qDebug() << "command:" << args.at(0);
    qDebug() << "host:" << host;
    qDebug() << "port:" << port;

    NerdiosRemote *remote = new NerdiosRemote();
    remote->run(args.at(0), host, port);

    return app.exec();
}

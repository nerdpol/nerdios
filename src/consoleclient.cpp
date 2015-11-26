#include "consoleclient.h"

#include <QXmppRosterManager.h>

ConsoleClient::ConsoleClient(QObject *parent)
    : QObject(parent)
    , out(stdout)
    , notifier(0, QSocketNotifier::Read)
{
    xmppclient.logger()->setLoggingType(QXmppLogger::StdoutLogging);
    QObject::connect(&notifier, SIGNAL(activated(int)), this, SLOT(readInput()));
    QObject::connect(&this->xmppclient, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(&this->xmppclient, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ConsoleClient::cleanScreen()
{
    out << "\E[H\E[2J";
}

void ConsoleClient::prolog()
{
    cleanScreen();
    out << "Hello and welcome to nerdios by nerdpol!" << endl;
    out << "----------------------------------------" << endl;
}

void ConsoleClient::connect(const QString& jid, const QString& password)
{
    //QObject::connect(&this->xmppclient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
    xmppclient.connectToServer(jid, password);
    //TODO(elnappo) add setClientPresence
}

void ConsoleClient::printRoster()
{
    foreach(const QString &id, xmppclient.rosterManager().getRosterBareJids()) {
        out << id << endl;
    }
}

void ConsoleClient::sendMessage(const QString recipient, const QString message)
{
    xmppclient.sendMessage(recipient, message);
    out << "<< " << recipient << ": " << message << endl << flush;
}

void ConsoleClient::exit()
{
    disconnect();
    epilog();
}

void ConsoleClient::disconnect()
{
    if (xmppclient.isConnected()) {
        xmppclient.disconnectFromServer();
    }
}

void ConsoleClient::epilog()
{
    out << "Made with <3 for Mike" << endl << flush;
}

void ConsoleClient::readInput()
{
    QTextStream in(stdin);
    QString input = in.readLine().trimmed();
    QStringList args = input.split(" ");

    if (args[0] == "connect") {
        if (args.length() != 3) {
            // help();
            return;
        }
        connect(args[1], args[2]);
    } else if (args[0] == "disconnect") {
        exit();
    }
}

void ConsoleClient::messageReceived(const QXmppMessage &message)
{
    out << ">> " << message.from() << ": " << message.body() << endl << flush;
}

void ConsoleClient::connected()
{
    out << "Connected to server" << endl << flush;
}

void ConsoleClient::disconnected()
{
    out << "Disconnected from server" << endl << flush;
}

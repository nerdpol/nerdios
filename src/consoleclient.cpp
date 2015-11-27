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
    QObject::connect(&this->xmppclient, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));
    QObject::connect(&this->xmppclient.rosterManager(), SIGNAL(rosterReceived()), this, SLOT(printRoster()));
    QObject::connect(this, SIGNAL(error(QString)), this, SLOT(printErrorMessage(QString)));
}

/**
 * cleans the console
 */
void ConsoleClient::cleanScreen()
{
    out << "\E[H\E[2J";
}

void ConsoleClient::prolog()
{
    cleanScreen();
    out << "Hello and welcome to nerdios by nerdpol!" << endl;
    out << "for help type help" << endl;
    out << "----------------------------------------" << endl;
}

void ConsoleClient::help()
{
    out << "Help:" << endl;
    out << "connect you@example.com mypassword" << endl;
    out << "roster" << endl;
    out << "msg friend@example.com Hallo my friend!" << endl;
    out << "disconnect" << endl;
    out << "exit" << endl;
}

void ConsoleClient::connect(const QString& jid, const QString& password)
{
    xmppclient.connectToServer(jid, password);
    xmppclient.setClientPresence(QXmppPresence::Available);
}

void ConsoleClient::printRoster()
{
    foreach(const QString &jid, xmppclient.rosterManager().getRosterBareJids()) {
        out << jid << endl;
    }
}

void ConsoleClient::sendMessage(const QString recipient, const QString message)
{
    if (!xmppclient.isConnected()) {
        emit error("not connected");
        return;
    }

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
            help();
            return;
        }
        connect(args[1], args[2]);
    } else if (args[0] == "roster") {
        printRoster();
    } else if (args[0] == "disconnect") {
        disconnect();
    } else if (args[0] == "exit") {
        exit();
    } else if (args[0] == "msg") {
        args.removeFirst();
        QString recipient = args[0];
        args.removeFirst();
        sendMessage(recipient, args.join(" "));
    } else if (args[0] == "status") {
        if (args[1] == "available") {
            xmppclient.setClientPresence(QXmppPresence::Available);
        } else if (args[1] == "away") {
            QXmppPresence p;
            p.setAvailableStatusType(QXmppPresence::Away);
            xmppclient.setClientPresence(p);
        }
    } else {
        help();
    }
}

void ConsoleClient::printErrorMessage(const QString &msg)
{
    out << msg << endl << flush;
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

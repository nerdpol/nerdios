#include "consoleclient.h"

ConsoleClient::ConsoleClient(QObject *parent)
    : QObject(parent)
    , in(stdin)
    , out(stdout)
    , xmppclient(NULL)
{

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

void ConsoleClient::connect()
{
    QString jid = "test@example.io";
    QString password = "password";

    /*
    out << "JID: " << flush;
    QString jid = in.readLine();

    out << "Password: " << flush;
    QString password = in.readLine();
    */

    xmppclient = new QXmppClient();
    xmppclient->logger()->setLoggingType(QXmppLogger::StdoutLogging);
    xmppclient->connectToServer(jid, password);
    out << "Connect to server" << endl << flush;
    //TODO(elnappo) add setClientPresence
}

void ConsoleClient::printRoster()
{

}

void ConsoleClient::sendMessage(const QString recipient, const QString message)
{
    xmppclient->sendMessage(recipient, message);
    out << "<< " << recipient << ": " << message << endl << flush;
}

void ConsoleClient::exit()
{
    disconnect();
    epilog();
}

void ConsoleClient::disconnect()
{
    xmppclient->disconnectFromServer();
    out << "Disconnected from server" << endl << flush;
}

void ConsoleClient::epilog()
{
    out << "Made with <3 for Mike" << endl << flush;
}

void ConsoleClient::messageReceived(const QXmppMessage &message)
{
    out << ">> " << message.from() << ": " << message.body() << endl << flush;
}

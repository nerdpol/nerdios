#include "tcpremoteserver.h"

TCPRemoteServer::TCPRemoteServer(NerdiosCore* core, QObject *parent)
    : QObject(parent)
    , m_core(core)
    , m_server(0)
    , m_serverPort(1337)
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(m_server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError(QAbstractSocket::SocketError)));

    if(!m_server->listen(QHostAddress::Any, m_serverPort))
    {
        qDebug() << "TCP remote server could not start!";
    }
    else
    {
        qDebug() << "TCP remote server started!";
    }
}

void TCPRemoteServer::sendContacts(QTextStream &stream)
{
    foreach (const QString &bareJid, m_core->roster()) {
        stream << bareJid << endl;
    }
    stream << flush;
}

void TCPRemoteServer::sendPresence(QTextStream &stream)
{
    stream << m_core->status()<< endl << flush;
}

void TCPRemoteServer::sendPriority(QTextStream &stream)
{
    stream << m_core->priority() << endl << flush;
}

void TCPRemoteServer::sendJID(QTextStream &stream)
{
    stream << m_core->jid() << endl << flush;
}

void TCPRemoteServer::sendState(QTextStream &stream)
{
    stream <<  m_core->state().toLower() << endl << flush;
}

void TCPRemoteServer::newConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();
    if (socket == NULL) {
        return;
    }

    QTextStream *stream = new QTextStream(socket);
    if (stream == NULL) {
        return;
    }

    socket->waitForReadyRead();
    QString line = stream->readLine();

    qDebug() << "Incomming line: " << line;
    if (line.isNull()) {
        return;
    }
    if (m_core->xmppClient()->isConnected()) {
        if (line == "contacts") {
            sendContacts(*stream);
        } else if (line == "status") {
            sendPresence(*stream);
        } else if (line == "priority") {
            sendPriority(*stream);
        } else if (line == "jid") {
            sendJID(*stream);
        } else if (line == "state") {
            sendState(*stream);
        } else {
            qDebug() << "Unknown command recived: " << line;
            *stream << "unknown_command_error" << endl << flush;
        }
    } else {
        qDebug() << "Recived command while not connected to a XMPP server.";
        *stream << "not_connected_error" << endl << flush;
    }

    socket->waitForBytesWritten(3000);
    socket->close();
}

void TCPRemoteServer::onAcceptError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "TCP server error on new connection:" << socketError;
}

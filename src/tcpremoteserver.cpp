#include "tcpremoteserver.h"

TCPRemoteServer::TCPRemoteServer(NerdiosCore* core, QObject *parent)
    : QObject(parent)
    , m_core(core)
    , m_server(0)
    , m_serverPort(1337)
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

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

    qDebug() << "Incomming: " << line;
    if (line.isNull()) {
        return;
    }
    if (m_core->xmppClient()->isConnected()) {
        if (line == "contacts") {
            sendContacts(*stream);
        }
    } else {
        *stream << "not_connected_error" << endl << flush;
    }

    socket->waitForBytesWritten(3000);
    socket->close();
}

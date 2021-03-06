#ifndef TCPREMOTE_H
#define TCPREMOTE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include "nerdioscore.h"

class TCPRemoteServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPRemoteServer(NerdiosCore* core, QObject *parent = 0);
    void sendContacts(QTextStream &stream);
    void sendPresence(QTextStream &stream);
    void sendPriority(QTextStream &stream);
    void sendJID(QTextStream &stream);
    void sendState(QTextStream &stream);

signals:

public slots:
    void newConnection();
    void onAcceptError(QAbstractSocket::SocketError socketError);

protected:
    NerdiosCore* m_core;
    QTcpServer* m_server;
    quint16 m_serverPort;
};

#endif // TCPREMOTE_H

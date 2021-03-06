#ifndef NERDIOSREMOTE_H
#define NERDIOSREMOTE_H

#include <QObject>
#include <QTextStream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

class NerdiosRemote : public QObject
{
    Q_OBJECT
public:
    explicit NerdiosRemote(QObject *parent = 0);

    void run(QString command, QString host = "localhost", int port = 1337);

signals:

public slots:
    void onSocketAvailable();
    void onReadyRead();
    void onTimeout();
    void onStateChanged(QAbstractSocket::SocketState socketState);
    void onError(QAbstractSocket::SocketError socketError);

protected:
    QTextStream m_out;
    QTextStream m_err;
    QString m_command;
    QString m_serverAddress;
    quint16 m_serverPort;
    QTcpSocket* m_socket;
    QTextStream m_stream;
    QTimer* m_timeout;
};

#endif // NERDIOSREMOTE_H

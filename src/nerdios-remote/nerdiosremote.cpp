#include "nerdiosremote.h"

NerdiosRemote::NerdiosRemote(QObject *parent)
    : QObject(parent)
    , m_out(stdout)
    , m_err(stderr)
    , m_serverPort(1337)
    , m_socket(0)
    , m_timeout(new QTimer(this))
{
    connect(m_timeout, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void NerdiosRemote::run(QString command, QString host, int port)
{
    m_command = command;
    m_serverAddress = host;
    m_serverPort = port;
    m_socket = new QTcpSocket(this);

    m_socket->connectToHost(m_serverAddress, m_serverPort);

    qDebug() << "Try to connect to server...";
    connect(m_socket, SIGNAL(connected()), this, SLOT(onSocketAvailable()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    m_timeout->start(10000); // 10 sec
}

void NerdiosRemote::onSocketAvailable()
{
    m_stream.setDevice(m_socket);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    qDebug() << "Send command: " << m_command;
    m_stream << m_command << endl << flush;
}

void NerdiosRemote::onReadyRead()
{
    while(true) {
        QString line = m_stream.readLine();
        qDebug() << "Incomming: " << line;
        if (line.isNull()) {
            break;
        }
        if (!line.endsWith("_error")) {
            m_out << line << endl << flush;
            m_socket->disconnectFromHost();
            exit(0);
        } else {
            m_err << line << endl << flush;
            m_socket->disconnectFromHost();
            exit(1);
        }
    }
}

void NerdiosRemote::onTimeout()
{
    m_err << "Error, timeout" << endl << flush;
    m_socket->disconnectFromHost();
    exit(1);
}

void NerdiosRemote::onStateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "State changed," << socketState;
}

void NerdiosRemote::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error," << socketError;
    QString errorMsg;

    switch (socketError) {
        case QAbstractSocket::ConnectionRefusedError:
            errorMsg = "Connection Refused";
            break;
        case QAbstractSocket::HostNotFoundError:
            errorMsg = "Host Not Found";
            break;
        case QAbstractSocket::SocketTimeoutError:
            errorMsg = "Socket Timeout";
            break;
        case QAbstractSocket::RemoteHostClosedError:
            return;
        default:
            errorMsg = "Connection Error";
            break;
    }
    m_err << errorMsg << endl << flush;
    m_socket->disconnectFromHost();
    exit(1);
}

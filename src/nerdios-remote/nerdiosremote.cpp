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

/**
 * @brief This is the only call needed to run nerdios-remote. All required
 * parameters are set here
 * @param command command to execute on remote nerdios-core
 * @param host host to connect to
 * @param port port to connect to
 */
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
    // Start global timeout to catch network timeouts
    m_timeout->start(10000); // 10 sec
}

/**
 * @brief on SocketAvailable from QTCPServer (m_socket) connect readyRead signal
 * and send command to execute to the remote QTCPServer running on nerdios-core
 */
void NerdiosRemote::onSocketAvailable()
{
    m_stream.setDevice(m_socket);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    qDebug() << "Send command: " << m_command;
    m_stream << m_command << endl << flush;
}

/**
 * @brief on readyRead from QTCPServer (m_socket) print answer from command
 * send in onSocketAvailable and exit with return code 0
 */
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
        } else {
            m_err << line << endl << flush;
            m_socket->disconnectFromHost();
            exit(1);
        }
    }
    m_socket->disconnectFromHost();
    exit(0);
}

/**
 * @brief on QTimer (m_timeout) timeout signal print error message to stderr
 *  and exit with return code 1
 */
void NerdiosRemote::onTimeout()
{
    m_err << "Error, timeout" << endl << flush;
    m_socket->disconnectFromHost();
    exit(1);
}

/**
 * @brief Print all socket state changes from QTCPClient (m_socket) if in debug mode
 * @param socketState
 */
void NerdiosRemote::onStateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "State changed," << socketState;
}

/**
 * @brief Handle errors signals from QTCPClient (m_socket). Print error message to stderr
 * and exit with return code 1
 * @param socketError
 */
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

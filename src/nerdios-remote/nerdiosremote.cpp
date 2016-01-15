#include "nerdiosremote.h"

NerdiosRemote::NerdiosRemote(QObject *parent)
    : QObject(parent)
    , m_out(stdout)
    , m_err(stderr)
    , m_serverPort(1337)
    , m_socket(0)
{

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
        } else {
            m_err << line << endl << flush;
        }
    }
}

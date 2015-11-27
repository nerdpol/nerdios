#include "nerdioscore.h"

NerdiosCore::NerdiosCore(QObject *parent)
    : QObject(parent)
    , m_xmppClient(new QXmppClient)
{
    m_xmppClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);
}

void NerdiosCore::setJID(QString jid)
{
    if (m_jid != jid) {
        m_jid = jid;
        emit jidChanged(jid);
    }
}

void NerdiosCore::setPassword(QString password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged(password);
    }
}

QString NerdiosCore::jid() const
{
    return m_jid;
}

QString NerdiosCore::password() const
{
    return m_password;
}

QXmppClient *NerdiosCore::xmppClient() const
{
    return m_xmppClient;
}

void NerdiosCore::connect()
{
    m_xmppClient->connectToServer(m_jid, m_password);
}

void NerdiosCore::disconnect()
{
    if (m_xmppClient->isConnected()) {
        m_xmppClient->disconnectFromServer();
    }
}

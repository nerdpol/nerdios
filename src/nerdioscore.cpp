#include "nerdioscore.h"
#include <QXmppRosterManager.h>
#include <QXmppMessage.h>

NerdiosCore::NerdiosCore(QObject *parent)
    : QObject(parent)
    , m_xmppClient(new QXmppClient(this))
    , m_trayIcon(new QSystemTrayIcon(this))
{
    m_xmppClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(onRosterChanged()));
    QObject::connect(this->m_xmppClient, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(onMessageReceived(QXmppMessage)));
    QObject::connect(this->m_xmppClient, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(this->m_xmppClient, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
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

QStringList NerdiosCore::roster() const
{
    return m_xmppClient->rosterManager().getRosterBareJids();
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

void NerdiosCore::sendMessage(const QString jid, const QString message)
{
    m_xmppClient->sendMessage(jid, message);
}

void NerdiosCore::addContact(const QString jid)
{
    m_xmppClient->rosterManager().addItem(jid);
}

void NerdiosCore::onRosterChanged()
{
    emit rosterChanged();
}

void NerdiosCore::onConnected()
{
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(presenceChanged()), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(subscriptionReceived()), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(itemAdded ()), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(itemChanged()), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(itemRemoved ()), this, SLOT(onRosterChanged()));
}

void NerdiosCore::onDisconnected()
{
    emit rosterChanged();
}

void NerdiosCore::onMessageReceived(const QXmppMessage &message)
{
    //QXMPPMessageQML messageQML(message);
    QXMPPMessageQML *messageQML = new QXMPPMessageQML(message, this);
    emit messageReceived(messageQML);
    m_trayIcon->showMessage(message.from(), message.body());
}

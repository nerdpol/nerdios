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
    QObject::connect(this->m_xmppClient, SIGNAL(stateChanged(QXmppClient::State)), this, SLOT(onStateChanged()));
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

void NerdiosCore::setLastUser(const QString &lastUser)
{
    if (m_lastUser != lastUser) {
        m_lastUser = lastUser;
        emit lastUserChanged(lastUser);
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

QString NerdiosCore::lastUser() const
{
    return m_lastUser;
}

QXmppClient *NerdiosCore::xmppClient() const
{
    return m_xmppClient;
}

QStringList NerdiosCore::roster() const
{
    return m_xmppClient->rosterManager().getRosterBareJids();
}

int NerdiosCore::priority() const
{
    return m_xmppClient->clientPresence().priority();
}

QString NerdiosCore::status() const
{
    switch (m_xmppClient->clientPresence().availableStatusType()) {
        case QXmppPresence::Online:
            return "Online";
        case QXmppPresence::Away:
            return "Away";
        case QXmppPresence::XA:
            return "XA";
        case QXmppPresence::DND:
            return "DND";
        case QXmppPresence::Chat:
            return "Chat";
        case QXmppPresence::Invisible:
            return "Invisible";
    }
}

QString NerdiosCore::state() const
{
    switch (m_xmppClient->state()) {
        case QXmppClient::State::ConnectedState:
            return "Connected";
        case QXmppClient::State::ConnectingState:
            return "Connecting";
        case QXmppClient::State::DisconnectedState:
            return "Disconnected";
    }
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

bool NerdiosCore::isConnected()
{
    return m_xmppClient->isConnected();
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
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(presenceChanged(const QString&, const QString&)), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(subscriptionReceived(const QString &bareJid)), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(itemAdded (const QString&)), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(itemChanged(const QString&)), this, SLOT(onRosterChanged()));
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(itemRemoved (const QString&)), this, SLOT(onRosterChanged()));
}

void NerdiosCore::onDisconnected()
{
    emit rosterChanged();
}

void NerdiosCore::onStateChanged()
{
    switch (m_xmppClient->state()) {
        case QXmppClient::State::ConnectedState:
            emit stateChanged("Connected");
        case QXmppClient::State::ConnectingState:
            emit stateChanged("Connecting");
        case QXmppClient::State::DisconnectedState:
            emit stateChanged("Disconnected");
    }
}

void NerdiosCore::onMessageReceived(const QXmppMessage &message)
{
    QXMPPMessageQML *messageQML = new QXMPPMessageQML(message, this);
    emit messageReceived(messageQML);
    m_trayIcon->showMessage(message.from(), message.body());
    m_lastUser = message.from();
    onRosterChanged();
}

void NerdiosCore::setStatus(const QString status)
{
    QXmppPresence presence;
    if (status == "available") {
        presence.setType(QXmppPresence::Type::Available);
        presence.setAvailableStatusType(QXmppPresence::AvailableStatusType::Online);
    } else if (status == "away") {
        presence.setType(QXmppPresence::Type::Available);
        presence.setAvailableStatusType(QXmppPresence::AvailableStatusType::Away);
    } else if (status == "do not disturb") {
        presence.setType(QXmppPresence::Type::Available);
        presence.setAvailableStatusType(QXmppPresence::AvailableStatusType::DND);
    } else {
        return;
    }

    m_xmppClient->setClientPresence(presence);
}

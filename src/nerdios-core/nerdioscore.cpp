#include "nerdioscore.h"
#include <QXmppRosterManager.h>
#include <QXmppMessage.h>
#include "utils.h"

#ifdef Q_OS_MACX
    #include <QtMac>
#endif

/**
 * @brief Main component of nerdios. Use this class in QML as the main interface
 * @param parent
 */
NerdiosCore::NerdiosCore(QObject *parent)
    : QObject(parent)
    , m_xmppClient(new QXmppClient(this))
    , m_trayIcon(new QSystemTrayIcon(this))
{
    //m_trayIcon->show();
    qDebug() << "QSystemTrayIcon.isSystemTrayAvailable:" << m_trayIcon->isSystemTrayAvailable();
    qDebug() << "QSystemTrayIcon.supportsMessages:" << m_trayIcon->supportsMessages();
    qDebug() << "QSystemTrayIcon.isVisible:" << m_trayIcon->isVisible();

    m_xmppClient->logger()->setLoggingType(QXmppLogger::StdoutLogging);
    // Connect some singals from qxmppClient
    QObject::connect(&this->m_xmppClient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(onRosterChanged()));
    QObject::connect(this->m_xmppClient, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(onMessageReceived(QXmppMessage)));
    QObject::connect(this->m_xmppClient, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(this->m_xmppClient, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(this->m_xmppClient, SIGNAL(stateChanged(QXmppClient::State)), this, SLOT(onStateChanged()));

    // auto-accept "friends"
    m_xmppClient->configuration().setAutoAcceptSubscriptions(true);
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

QXmppRosterManager *NerdiosCore::rosterManager() const
{
    if (m_xmppClient) {
        QXmppRosterManager &rosterManager = m_xmppClient->rosterManager();
        return &rosterManager;
    }

    return NULL;
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
    return availableStatusTypeToString(m_xmppClient->clientPresence().availableStatusType());
}

QString NerdiosCore::state() const
{
    return stateToString(m_xmppClient->state());
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
    if (isValidJid(jid)) {
        m_xmppClient->rosterManager().addItem(jid);
    } else {
        qDebug() << "invalid jid:" << jid;
    }
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
    emit stateChanged(this->state());
}

void NerdiosCore::onMessageReceived(const QXmppMessage &message)
{
#ifdef Q_OS_MACX
    QtMac::setBadgeLabelText("1");
#endif
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
    } else if (status == "xa") {
        presence.setType(QXmppPresence::Type::Available);
        presence.setAvailableStatusType(QXmppPresence::AvailableStatusType::XA);
    } else if (status == "chat") {
        presence.setType(QXmppPresence::Type::Available);
        presence.setAvailableStatusType(QXmppPresence::AvailableStatusType::Chat);
    } else {
        return;
    }

    m_xmppClient->setClientPresence(presence);
    emit statusChanged(this->status());
}

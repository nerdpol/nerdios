#include <QXmppRosterManager.h>

#include "contactlist.h"
#include "utils.h"

ContactList::ContactList(QObject *parent)
    : QAbstractListModel(parent)
{

}

ContactList::~ContactList()
{

}

void ContactList::setXmppClient(QXmppClient *xmppClient)
{
    if (m_xmppClient == xmppClient) {
        return;
    }

    if (m_xmppClient) {
        // disconnect
        disconnect(&(m_xmppClient->rosterManager()), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
        disconnect(&(m_xmppClient->rosterManager()), SIGNAL(presenceChanged(QString,QString)), this, SLOT(presenceChanged(QString,QString)));
        disconnect(&(m_xmppClient->rosterManager()), SIGNAL(subscriptionReceived(QString)), this, SLOT(subscriptionReceived(QString)));
        disconnect(&(m_xmppClient->rosterManager()), SIGNAL(itemAdded(QString)), this, SLOT(itemAdded(QString)));
        disconnect(&(m_xmppClient->rosterManager()), SIGNAL(itemChanged(QString)), this, SLOT(itemChanged(QString)));
        disconnect(&(m_xmppClient->rosterManager()), SIGNAL(itemRemoved(QString)), this, SLOT(itemRemoved(QString)));
        disconnect(m_xmppClient, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));
    }

    m_xmppClient = xmppClient;

    if (m_xmppClient) {
        // connect
        connect(&(m_xmppClient->rosterManager()), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
        connect(&(m_xmppClient->rosterManager()), SIGNAL(presenceChanged(QString,QString)), this, SLOT(presenceChanged(QString,QString)));
        connect(&(m_xmppClient->rosterManager()), SIGNAL(subscriptionReceived(QString)), this, SLOT(subscriptionReceived(QString)));
        connect(&(m_xmppClient->rosterManager()), SIGNAL(itemAdded(QString)), this, SLOT(itemAdded(QString)));
        connect(&(m_xmppClient->rosterManager()), SIGNAL(itemChanged(QString)), this, SLOT(itemChanged(QString)));
        connect(&(m_xmppClient->rosterManager()), SIGNAL(itemRemoved(QString)), this, SLOT(itemRemoved(QString)));
        connect(m_xmppClient, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));
    }

    emit xmppClientChanged();
    reset();
}

QXmppClient *ContactList::xmppClient() const
{
    return m_xmppClient;
}

int ContactList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_contacts.count();
}

QVariant ContactList::data(const QModelIndex &index, int role) const
{
    QVariantMap map = m_contacts.at(index.row());

    switch (role) {
    case JidRole:
        return map.value("jid");
    case TypeRole:
        return map.value("type");
    case AvailableTypeRole:
        return map.value("available_type");
    }

    return QVariant();
}

QHash<int, QByteArray> ContactList::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(JidRole, "jid");
    roleNames.insert(TypeRole, "type");
    roleNames.insert(AvailableTypeRole, "available_type");
    return roleNames;

}

void ContactList::rosterReceived()
{
}

void ContactList::presenceChanged(const QString &bareJid, const QString &resource)
{
    Q_UNUSED(bareJid)
    Q_UNUSED(resource)
    reset();
}

void ContactList::subscriptionReceived(const QString &bareJid)
{
    Q_UNUSED(bareJid)
    reset();
}

void ContactList::itemAdded(const QString &bareJid)
{
    Q_UNUSED(bareJid)
    reset();
}

void ContactList::itemChanged(const QString &bareJid)
{
    Q_UNUSED(bareJid)
    reset();
}

void ContactList::itemRemoved(const QString &bareJid)
{
    Q_UNUSED(bareJid)
    reset();
}

void ContactList::messageReceived(const QXmppMessage &message)
{
    Q_UNUSED(message)
    reset();
}

void ContactList::reset()
{
    beginResetModel();

    if (m_xmppClient) {
        m_contacts.clear();
        foreach (const QString &jid, m_xmppClient->rosterManager().getRosterBareJids()) {
            QVariantMap map;
            QMap<QString, QXmppPresence> presences = m_xmppClient->rosterManager().getAllPresencesForBareJid(jid);

            map.insert("jid", jid);
            // We only care about one resource (at the moment), and that's the first.
            if (!presences.isEmpty()) {
                map.insert("type", typeToString(presences.first().type()));
                map.insert("available_type", availableStatusTypeToString(presences.first().availableStatusType()));
            } else {
                map.insert("type", "");
                map.insert("available_type", "");
            }
            m_contacts.append(map);
        }
    }

    endResetModel();
}

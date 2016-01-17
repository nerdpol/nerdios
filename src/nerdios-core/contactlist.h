#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QObject>
#include <QtGlobal>
#include <QAbstractListModel>

#include "QXmppClient.h"

class ContactList : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)
    Q_PROPERTY(QXmppClient* xmppClient READ xmppClient WRITE setXmppClient NOTIFY xmppClientChanged)

public:
    enum Roles {
        JidRole = Qt::UserRole + 1,
        TypeRole,
        AvailableTypeRole
    };

    ContactList(QObject *parent = 0);
    ~ContactList();

    void setXmppClient(QXmppClient *xmppClient);
    QXmppClient *xmppClient() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void reset();

signals:
    void xmppClientChanged();

public slots:
    void rosterReceived();
    void presenceChanged(const QString& bareJid, const QString& resource);
    void subscriptionReceived(const QString& bareJid);
    void itemAdded(const QString& bareJid);
    void itemChanged(const QString& bareJid);
    void itemRemoved(const QString& bareJid);

protected:
    QList<QVariantMap> m_contacts;
    QXmppClient *m_xmppClient;
};

#endif // CONTACTLIST_H

#ifndef NERDIOSCORE_H
#define NERDIOSCORE_H

#include <QObject>
#include <QtGlobal>
#include <QSystemTrayIcon>
#include <QXmppClient.h>

#include "qxmppmessageqml.h"


class NerdiosCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString jid READ jid WRITE setJID NOTIFY jidChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QXmppClient* xmppClient READ xmppClient CONSTANT)
    Q_PROPERTY(QStringList roster READ roster NOTIFY rosterChanged)

public:
    explicit NerdiosCore(QObject *parent = 0);

    void setJID(const QString jid);
    void setPassword(const QString password);

    QString jid() const;
    QString password() const;
    QXmppClient* xmppClient() const;
    QStringList roster() const;

    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendMessage(const QString jid, const QString message);

signals:
    void jidChanged(const QString jid);
    void passwordChanged(const QString passwird);
    void rosterChanged();
    void messageReceived(QXMPPMessageQML* message);

public slots:
    void onRosterChanged();
    void onConnected();

protected slots:
    void onMessageReceived(const QXmppMessage& message);

protected:
    QXmppClient* m_xmppClient;
    QString m_jid;
    QString m_password;
    QSystemTrayIcon* m_trayIcon;

};

#endif // NERDIOSCORE_H

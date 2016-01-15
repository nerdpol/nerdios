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
    Q_PROPERTY(QString lastUser READ lastUser WRITE setLastUser NOTIFY lastUserChanged)
    Q_PROPERTY(QString jid READ jid WRITE setJID NOTIFY jidChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QXmppClient* xmppClient READ xmppClient CONSTANT)
    Q_PROPERTY(QStringList roster READ roster NOTIFY rosterChanged)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit NerdiosCore(QObject *parent = 0);

    void setJID(const QString jid);
    void setPassword(const QString password);
    void setLastUser(const QString& lastUser);

    QString jid() const;
    QString password() const;
    QString lastUser() const;
    QXmppClient* xmppClient() const;
    QStringList roster() const;
    int priority() const;
    QString status() const;
    QString state() const;

    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE bool isConnected();
    Q_INVOKABLE void sendMessage(const QString jid, const QString message);
    Q_INVOKABLE void addContact(const QString jid);
    Q_INVOKABLE void setStatus(const QString status);

signals:
    void jidChanged(const QString jid);
    void passwordChanged(const QString password);
    void lastUserChanged(const QString &lastUser);
    void rosterChanged();
    void messageReceived(QXMPPMessageQML* message);
    void stateChanged(QString state);
    void statusChanged(QString status);

public slots:
    void onRosterChanged();
    void onConnected();
    void onDisconnected();
    void onStateChanged();

protected slots:
    void onMessageReceived(const QXmppMessage& message);

protected:
    QXmppClient* m_xmppClient;
    QString m_jid;
    QString m_password;
    QSystemTrayIcon* m_trayIcon;
    QString m_lastUser;

};

#endif // NERDIOSCORE_H

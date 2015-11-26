#ifndef NERDIOSCORE_H
#define NERDIOSCORE_H

#include <QObject>
#include <QtGlobal>
#include <QXmppClient.h>

class NerdiosCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString jid READ jid WRITE setJID NOTIFY jidChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QXmppClient* xmppClient READ xmppClient CONSTANT)

public:
    explicit NerdiosCore(QObject *parent = 0);

    void setJID(const QString jid);
    void setPassword(const QString password);

    QString jid() const;
    QString password() const;
    QXmppClient* xmppClient() const;

    Q_INVOKABLE void connect();
    Q_INVOKABLE void disconnect();

signals:
    void jidChanged(const QString jid);
    void passwordChanged(const QString passwird);

public slots:

protected:
    QXmppClient* m_xmppClient;
    QString m_jid;
    QString m_password;

};

#endif // NERDIOSCORE_H

#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include <QObject>
#include <QTextStream>
#include <QSocketNotifier>

#include "QXmppClient.h"
#include "QXmppMessage.h"
#include "QXmppLogger.h"


class ConsoleClient : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleClient(QObject *parent = 0);
    void cleanScreen();
    void prolog();
    void help();
    void connect(const QString &jid, const QString &password);
    void sendMessage(const QString recipient, const QString message);
    void exit();
    void disconnect();
    void epilog();

signals:

public slots:
    void messageReceived(const QXmppMessage& message);
    void printRoster();
    void connected();
    void disconnected();
    void readInput();

protected:
    QTextStream out;
    QXmppClient xmppclient;
    QSocketNotifier notifier;
};

#endif // CONSOLECLIENT_H

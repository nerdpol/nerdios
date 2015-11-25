#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include <QObject>
#include <QTextStream>

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
    void connect();
    void printRoster();
    void sendMessage(const QString recipient, const QString message);
    void exit();
    void disconnect();
    void epilog();

signals:

public slots:
    void messageReceived(const QXmppMessage& message);

protected:
    QTextStream in;
    QTextStream out;
    QXmppClient *xmppclient;
};

#endif // CONSOLECLIENT_H

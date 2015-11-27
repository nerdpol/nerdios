#ifndef QXMPPMESSAGEQML_H
#define QXMPPMESSAGEQML_H

#include <QObject>
#include <QXmppMessage.h>


class QXMPPMessageQML : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString body READ body CONSTANT)
    Q_PROPERTY(QString from READ from CONSTANT)

public:
    explicit QXMPPMessageQML(QObject *parent = 0);
    QXMPPMessageQML(const QXmppMessage &message, QObject *parent = 0);

    QString body() const;
    QString from() const;

signals:

public slots:

protected:
    QXmppMessage m_qxmppMessage;

};

#endif // QXMPPMESSAGEQML_H

#include "qxmppmessageqml.h"

QXMPPMessageQML::QXMPPMessageQML(QObject *parent)
    : QObject(parent)
{

}

QXMPPMessageQML::QXMPPMessageQML(const QXmppMessage &message, QObject *parent)
    : QObject(parent)
    , m_qxmppMessage(message)
{

}

QString QXMPPMessageQML::body() const
{
    return m_qxmppMessage.body();
}

QString QXMPPMessageQML::receiptId() const
{
    return m_qxmppMessage.receiptId();
}

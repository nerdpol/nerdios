#ifndef UTILS_H
#define UTILS_H

#include <QString>

#include "QXmppClient.h"
#include "QXmppPresence.h"

/**
 * @brief Convert a QXmppPresence::Type to a human readable QString
 * @param type
 * @return QString
 */
static QString typeToString(QXmppPresence::Type type)
{
    switch (type) {
        case QXmppPresence::Error:
            return "Error";
        case QXmppPresence::Available:
            return "Available";
        case QXmppPresence::Unavailable:
            return "Unavailable";
        case QXmppPresence::Subscribe:
            return "Subscribe";
        case QXmppPresence::Subscribed:
            return "Subscribed";
        case QXmppPresence::Unsubscribe:
            return "Unsubscribe";
        case QXmppPresence::Unsubscribed:
            return "Unsubscribed";
        case QXmppPresence::Probe:
            return "Probe";
        default:
            return QString();
    }
}

/**
 * @brief Convert a QXmppPresence::AvailableStatusType to a human readable QString
 * @param availableStatusType
 * @return QString
 */
static QString availableStatusTypeToString(QXmppPresence::AvailableStatusType availableStatusType)
{
    switch (availableStatusType) {
        case QXmppPresence::Online:
            return "Online";
        case QXmppPresence::Away:
            return "Away";
        case QXmppPresence::XA:
            return "XA";
        case QXmppPresence::DND:
            return "DND";
        case QXmppPresence::Chat:
            return "Chat";
        case QXmppPresence::Invisible:
            return "Invisible";
        default:
            return QString();
    }
}

/**
 * @brief Convert a QXmppClient::State to a human readable QString
 * @param state
 * @return QString
 */
static QString stateToString(QXmppClient::State state)
{
    switch (state) {
        case QXmppClient::State::ConnectedState:
            return "Connected";
        case QXmppClient::State::ConnectingState:
            return "Connecting";
        case QXmppClient::State::DisconnectedState:
            return "Disconnected";
        default:
            return QString();
    }
}

/**
 * @brief Validate a given jid with an email regex
 * @param jid barejid to validate
 * @return true if given jid is a valid jid else false
 */
static bool isValidJid(const QString jid)
{
    QRegExp regex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");
    return regex.exactMatch(jid);
}

#endif

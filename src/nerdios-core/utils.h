#ifndef UTILS_H
#define UTILS_H

#include <QString>

#include "QXmppClient.h"
#include "QXmppPresence.h"

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
    }

    return QString();
}

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
    }

    return QString();
}

static QString stateToString(QXmppClient::State state)
{
    switch (state) {
    case QXmppClient::State::ConnectedState:
        return "Connected";
    case QXmppClient::State::ConnectingState:
        return "Connecting";
    case QXmppClient::State::DisconnectedState:
        return "Disconnected";
    }

    return QString();
}

#endif

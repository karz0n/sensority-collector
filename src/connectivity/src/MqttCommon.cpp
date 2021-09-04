#include "connectivity/MqttCommon.hpp"

#include <mosquitto.h>

#include <Poco/Exception.h>

#include <cstring>

namespace connectivity {

std::string
stringifyErrorCode(int errorCode)
{
    const char* s = mosquitto_strerror(errorCode);
    return std::string{s};
}

std::string
stringifyConnAckCode(int connAckCode)
{
    const char* s = mosquitto_connack_string(connAckCode);
    return std::string{s};
}

void
convert(const int from, MqttLogLevel& to)
{
    switch (from) {
    case MOSQ_LOG_NONE:
        to = MqttLogLevel::None;
    case MOSQ_LOG_INFO:
        to = MqttLogLevel::Info;
        break;
    case MOSQ_LOG_NOTICE:
        to = MqttLogLevel::Notice;
        break;
    case MOSQ_LOG_WARNING:
        to = MqttLogLevel::Warning;
        break;
    case MOSQ_LOG_ERR:
        to = MqttLogLevel::Error;
        break;
    case MOSQ_LOG_DEBUG:
        to = MqttLogLevel::Debug;
        break;
    case MOSQ_LOG_SUBSCRIBE:
        to = MqttLogLevel::Subscribe;
        break;
    case MOSQ_LOG_UNSUBSCRIBE:
        to = MqttLogLevel::Unsubscribe;
        break;
    case MOSQ_LOG_WEBSOCKETS:
        to = MqttLogLevel::Websockets;
        break;
    default:
#ifndef NDEBUG
        poco_unexpected();
#else
        to = MqttLogLevel::None;
#endif
    }
}

void
convert(const int from, MqttConnectionStatusCodes& to)
{
    /**
     * Used here status codes are valid only for v3.1.1 MQTT protocol version
     */
    switch (from) {
    case 0: // Connection accepted
        to = MqttConnectionStatusCodes::Success;
        break;
    case 1: // The Server does not support the level of the MQTT protocol requested by the Client
        to = MqttConnectionStatusCodes::UnacceptableProtocolVersion;
        break;
    case 2: // The Client identifier is correct UTF-8 but not allowed by the Server
        to = MqttConnectionStatusCodes::IdentifierRejected;
        break;
    case 3: // The Network Connection has been made but the MQTT service is unavailable
        to = MqttConnectionStatusCodes::ServerUnavailable;
        break;
    case 4: // The data in the username or password is malformed
        to = MqttConnectionStatusCodes::BadCredentials;
        break;
    case 5: // The Client is not authorized to connect
        to = MqttConnectionStatusCodes::NotAuthorized;
        break;
    default:
#ifndef NDEBUG
        poco_unexpected();
#else
        to = MqttConnectionStatusCodes::Unknown;
#endif
    }
}

void
convert(const mosquitto_message* from, MqttMessage& to)
{
    to.id = from->mid;
    to.topic.assign(from->topic);
    if (from->payloadlen > 0) {
        to.payload.resize(from->payloadlen);
        std::memcpy(to.payload.data(), from->payload, from->payloadlen);
    } else {
        to.payload.clear();
    }
    to.qos = from->qos;
    to.retain = from->retain;
}

} // namespace connectivity
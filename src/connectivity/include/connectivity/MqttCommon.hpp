#pragma once

#include <vector>
#include <string>
#include <string_view>

struct mosquitto;
struct mosquitto_message;

namespace connectivity {

constexpr unsigned short MqttDefaultInsecurePort = 1883;

constexpr unsigned short MqttDefaultSecurePort = 8883;

constexpr unsigned short InvalidMessageId = 0;

constexpr int MqttDefaultKeepAlive = 60;

constexpr std::string_view MqttTLSv1{"tlsv1"};

constexpr std::string_view MqttTLSv11{"tlsv1.1"};

constexpr std::string_view MqttTLSv12{"tlsv1.2"};

enum class MqttLogLevel {
    None,
    Info,
    Notice,
    Warning,
    Error,
    Debug,
    Subscribe,
    Unsubscribe,
    Websockets
};

enum class MqttConnectionStatusCodes {
    Success,
    UnacceptableProtocolVersion,
    IdentifierRejected,
    ServerUnavailable,
    BadCredentials,
    NotAuthorized,
    Unknown
};

enum class MqttVerificationRequirements { None, Peer };

enum class MqttTransportLayerVersions { v1, v11, v12 };

enum class MqttQoS { v0 = 0, v1, v2 };

enum class MqttProtocolVersion { v31, v311 };

using MqttContext = mosquitto;

struct MqttMessage {
    int id{0};
    std::string topic;
    std::vector<std::uint8_t> payload;
    int qos{0};
    bool retain{false};

    [[nodiscard]] bool
    empty() const
    {
        return payload.empty();
    }
};

using MqttGrantedQoS = std::vector<int>;

std::string
stringifyErrorCode(int errorCode);

std::string
stringifyConnAckCode(int connAckCode);

void
convert(int from, MqttLogLevel& to);

void
convert(int from, MqttConnectionStatusCodes& to);

void
convert(const mosquitto_message* from, MqttMessage& to);

} // namespace connectivity
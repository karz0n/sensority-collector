#include "connectivity/MqttClient.hpp"

#include "common/Logger.hpp"

#include <mosquitto.h>

#include <Poco/Exception.h>
#include <Poco/Format.h>
#include <Poco/UUIDGenerator.h>

namespace {
template<typename T, typename M = std::string>
inline T
makeException(M&& message, int errorCode)
{
    std::string s = connectivity::stringifyErrorCode(errorCode);
    T nested{s, errorCode};
    return T{std::forward<M>(message), nested};
}
} // namespace

namespace connectivity {

MqttClient::MqttClient(const bool cleanSession /*= true*/)
    : MqttClient{Poco::UUIDGenerator::defaultGenerator().createRandom().toString(), cleanSession}
{
}

MqttClient::MqttClient(const std::string& id, const bool cleanSession /*= true*/)
    : _context{nullptr}
    , _durable{!cleanSession}
    , _connected{false}
    , _active{false}
{
    _context = mosquitto_new(id.data(), cleanSession, this);
    if (_context == nullptr) {
        const auto msg = Poco::format("Create mosquitto internal object with '%s' id", id);
        throw Poco::RuntimeException(msg, id);
    }

    mosquitto_connect_callback_set(_context, &onConnect);
    mosquitto_disconnect_callback_set(_context, &onDisconnect);
    mosquitto_publish_callback_set(_context, &onPublish);
    mosquitto_message_callback_set(_context, &onMessage);
    mosquitto_subscribe_callback_set(_context, &onSubscribe);
    mosquitto_unsubscribe_callback_set(_context, &onUnsubscribe);
    mosquitto_log_callback_set(_context, &onLog);
}

MqttClient::~MqttClient()
{
    if (_connected) {
        mosquitto_loop_stop(_context, true);
    }
    mosquitto_destroy(_context);
}

bool
MqttClient::durable() const noexcept
{
    return _durable;
}

bool
MqttClient::connected() const noexcept
{
    return _connected;
}

void
MqttClient::connect(const std::string& host,
                    const unsigned short port /*= MqttDefaultInsecurePort*/,
                    const int keepalive /*= MqttDefaultKeepAlive*/)
{
    poco_assert_msg(!_connected, "Client is connected");

    int rc = mosquitto_connect_async(_context, host.data(), port, keepalive);
    if (rc != MOSQ_ERR_SUCCESS) {
        const auto msg = Poco::format("Failed connect to '%s:%d' server", host, port);
        throw makeException<Poco::RuntimeException>(msg, rc);
    }

    if (!_active) {
        rc = mosquitto_loop_start(_context);
        if (rc != MOSQ_ERR_SUCCESS) {
            mosquitto_disconnect(_context);
            throw makeException<Poco::RuntimeException>("Failed start thread loop", rc);
        }
        _active = true;
    }
}

void
MqttClient::disconnect()
{
    if (_connected) {
        int rc = mosquitto_disconnect(_context);
        if (rc != MOSQ_ERR_SUCCESS) {
            throw makeException<Poco::RuntimeException>("Disconnect from host", rc);
        }
    }
}

void
MqttClient::setCredentials(const std::string& username, const std::string& password)
{
    poco_assert_msg(!_connected, "Client is connected");

    poco_assert_dbg(!username.empty());
    poco_assert_dbg(!password.empty());

    int rc = mosquitto_username_pw_set(_context, username.data(), password.data());
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Set credential", rc);
    }
}

void
MqttClient::setReconnectDelay(const unsigned int delay,
                              const unsigned int delayMax,
                              const bool exponentialBackoff)
{
    int rc = mosquitto_reconnect_delay_set(_context, delay, delayMax, exponentialBackoff);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Set reconnect delay", rc);
    }
}

void
MqttClient::setInsecure(const bool insecure)
{
    poco_assert_msg(!_connected, "Client is connected");

    int rc = mosquitto_tls_insecure_set(_context, insecure);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Set insecure mode", rc);
    }
}

void
MqttClient::setTransportLayerVersion(const MqttVerificationRequirements requirements,
                                     const MqttTransportLayerVersions version)
{
    poco_assert_msg(!_connected, "Client is connected");

    int req = (requirements == MqttVerificationRequirements::Peer) ? 0  // SSL_VERIFY_NONE
                                                                   : 1; // SSL_VERIFY_PEER

    std::string versionOut;
    switch (version) {
    case MqttTransportLayerVersions::v1:
        versionOut = MqttTLSv1;
        break;
    case MqttTransportLayerVersions::v11:
        versionOut = MqttTLSv11;
        break;
    case MqttTransportLayerVersions::v12:
        versionOut = MqttTLSv12;
        break;
    default:
        throw Poco::RuntimeException{"Unexpected TLS version"};
    }

    int rc = mosquitto_tls_opts_set(_context, req, versionOut.data(), nullptr);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Set transport layer options", rc);
    }
}

void
MqttClient::configureTransportLayerSecurity(const std::string& path)
{
    poco_assert_msg(!_connected, "Client is connected");

    int rc = mosquitto_tls_set(_context, nullptr, path.data(), nullptr, nullptr, nullptr);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Configure transport layer security", rc);
    }
}

void
MqttClient::configureTransportLayerSecurity(const std::string& psk,
                                            const std::string& identity,
                                            const std::string& ciphers)
{
    poco_assert_msg(!_connected, "Client is connected");

    int rc = mosquitto_tls_psk_set(_context, psk.data(), identity.data(), ciphers.data());
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Configure transport layer security", rc);
    }
}

void
MqttClient::configureTransportLayerSecurity(
    const std::string& caFile,
    const std::string& certFile,
    const std::string& keyFile,
    std::optional<PasswordProviderCallback> passwordProvider)
{
    poco_assert_msg(!_connected, "Client is connected");

    int rc = mosquitto_tls_set(_context,
                               caFile.data(),
                               nullptr,
                               certFile.data(),
                               keyFile.data(),
                               passwordProvider.value_or(nullptr));
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Configure transport layer security", rc);
    }
}

void
MqttClient::setProtocolVersion(const MqttProtocolVersion version)
{
    poco_assert_msg(!_connected, "Client is connected");

    int versionOut = (version == MqttProtocolVersion::v31) ? MQTT_PROTOCOL_V31 : MQTT_PROTOCOL_V311;
    int rc = mosquitto_int_option(_context, MOSQ_OPT_PROTOCOL_VERSION, versionOut);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Set client protocol version", rc);
    }
}

void
MqttClient::willSet(const std::string& topic,
                    const void* const payload,
                    const std::size_t size,
                    const MqttQoS qos /*= MqttQoS::v0*/,
                    const bool retain /*= false*/)
{
    poco_assert_msg(!_connected, "Client is connected");

    poco_assert_dbg(!topic.empty());
    poco_assert_dbg(payload != nullptr && size > 0);

    int rc = mosquitto_will_set(
        _context, topic.data(), static_cast<int>(size), payload, static_cast<int>(qos), retain);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Set will message", rc);
    }
}

void
MqttClient::willSet(const std::string& topic,
                    const std::string& message,
                    const MqttQoS qos /*= MqttQoS::v0*/,
                    const bool retain /*= false*/)
{
    willSet(topic, message.data(), message.size(), qos, retain);
}

void
MqttClient::willClear()
{
    int rc = mosquitto_will_clear(_context);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Clear will message", rc);
    }
}

int
MqttClient::publish(const std::string& topic,
                    const void* const payload,
                    const std::size_t size,
                    const MqttQoS qos /*= MqttQoS::v0*/,
                    const bool retain /*= false*/)
{
    poco_assert_dbg(!topic.empty());
    poco_assert_dbg(payload != nullptr && size > 0);

    int mid{0};
    int rc = mosquitto_publish(_context,
                               &mid,
                               topic.data(),
                               static_cast<int>(size),
                               payload,
                               static_cast<int>(qos),
                               retain);
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Publish data to the broker", rc);
    }
    return mid;
}

int
MqttClient::publish(const std::string& topic,
                    const std::string& message,
                    const MqttQoS qos /*= MqttQoS::v0*/,
                    const bool retain /*= false*/)
{
    return publish(topic, message.data(), message.size(), qos, retain);
}

int
MqttClient::subscribe(const std::string& topic, const MqttQoS qos /*= MqttQoS::v0*/)
{
    poco_assert_dbg(!topic.empty());

    int mid{0};
    int rc = mosquitto_subscribe(_context, &mid, topic.data(), static_cast<int>(qos));
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Subscribe to the topic", rc);
    }
    return mid;
}

int
MqttClient::unsubscribe(const std::string& topic)
{
    poco_assert_dbg(!topic.empty());

    int mid{0};
    int rc = mosquitto_unsubscribe(_context, &mid, topic.data());
    if (rc != MOSQ_ERR_SUCCESS) {
        throw makeException<Poco::RuntimeException>("Unsubscribe from the topic", rc);
    }
    return mid;
}

bs::connection
MqttClient::onConnect(const OnConnectSignal::slot_type& slot)
{
    return _connectSig.connect(slot);
}

bs::connection
MqttClient::onDisconnect(const OnDisconnectSignal::slot_type& slot)
{
    return _disconnectSig.connect(slot);
}

bs::connection
MqttClient::onPublish(const OnPublishSignal::slot_type& slot)
{
    return _publishSig.connect(slot);
}

bs::connection
MqttClient::onMessage(const OnMessageSignal::slot_type& slot)
{
    return _messageSig.connect(slot);
}

bs::connection
MqttClient::onSubscribe(const OnSubscribeSignal::slot_type& slot)
{
    return _subscribeSig.connect(slot);
}

bs::connection
MqttClient::onUnsubscribe(const OnUnsubscribeSignal::slot_type& slot)
{
    return _unsubscribeSig.connect(slot);
}

bs::connection
MqttClient::onLog(const OnLogSignal::slot_type& slot)
{
    return _logSig.connect(slot);
}

void
MqttClient::onConnect(MqttContext*, void* const object, const int status)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    auto statusCode{MqttConnectionStatusCodes::Unknown};
    convert(status, statusCode);
    if (statusCode == MqttConnectionStatusCodes::Success) {
        client._connected = true;
        client.emitConnectSignal();
    } else {
        client._connected = false;
        LOG_ERROR_F("Connecting has failed with <%d> status", status);
    }
}

void
MqttClient::onDisconnect(MqttContext*, void* const object, const int reason)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    client._connected = false;

    client.emitDisconnectSignal(reason);
}

void
MqttClient::onPublish(MqttContext*, void* const object, const int messageId)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    client.emitPublishSignal(messageId);
}

void
MqttClient::onMessage(MqttContext*, void* const object, const mosquitto_message* message)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    MqttMessage messageOut;
    convert(message, messageOut);

    client.emitMessageSignal(messageOut);
}

void
MqttClient::onSubscribe(MqttContext*,
                        void* const object,
                        const int messageId,
                        const int qosCount,
                        const int* grantedQos)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    MqttGrantedQoS grantedQosOut;
    if (qosCount > 0 && grantedQos != nullptr) {
        grantedQosOut.assign(grantedQos, grantedQos + qosCount);
    }

    client.emitSubscribeSignal(messageId, grantedQosOut);
}

void
MqttClient::onUnsubscribe(MqttContext*, void* const object, const int messageId)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    client.emitUnsubscribeSignal(messageId);
}

void
MqttClient::onLog(MqttContext*, void* object, const int level, const char* const message)
{
    poco_check_ptr(object);
    MqttClient& client = *static_cast<MqttClient*>(object);

    auto levelOut{MqttLogLevel::None};
    convert(level, levelOut);

    client.emitLogSignal(levelOut, std::string{message});
}

void
MqttClient::emitConnectSignal()
{
    LOG_DEBUG("OnConnect");

    _connectSig();
}

void
MqttClient::emitDisconnectSignal(const int reason)
{
    LOG_DEBUG_F("OnDisconnect: reason<%d>", reason);

    _disconnectSig(reason);
}

void
MqttClient::emitPublishSignal(const int messageId)
{
    LOG_DEBUG_F("OnPublish: mid<%d>", messageId);

    _publishSig(messageId);
}

void
MqttClient::emitMessageSignal(const MqttMessage& message)
{
    LOG_DEBUG_F("OnMessage: id<%d>, topic<%s>", message.id, message.topic);

    _messageSig(message);
}

void
MqttClient::emitSubscribeSignal(const int messageId, const MqttGrantedQoS& qos)
{
    LOG_DEBUG_F("OnSubscribe: mid<%d>", messageId);

    _subscribeSig(messageId, qos);
}

void
MqttClient::emitUnsubscribeSignal(const int messageId)
{
    LOG_DEBUG_F("OnUnsubscribe: mid<%d>", messageId);

    _unsubscribeSig(messageId);
}

void
MqttClient::emitLogSignal(const MqttLogLevel level, const std::string& message)
{
    LOG_DEBUG_F("OnLog: message<%s>", message);

    _logSig(level, message);
}

} // namespace connectivity
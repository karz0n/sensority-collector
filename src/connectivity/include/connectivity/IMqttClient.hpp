#pragma once

#include "connectivity/MqttCommon.hpp"

#include <boost/signals2/signal.hpp>

#include <string>
#include <memory>
#include <optional>

namespace bs = boost::signals2;

namespace connectivity {

class IMqttClient {
public:
    using Ptr = std::shared_ptr<IMqttClient>;
    using PasswordProviderCallback = int (*)(char*, int, int, void*);

    /* Signals */
    using OnConnectSignal = bs::signal<void()>;
    using OnDisconnectSignal = bs::signal<void(const int reason)>;
    using OnPublishSignal = bs::signal<void(const int messageId)>;
    using OnMessageSignal = bs::signal<void(const MqttMessage&)>;
    using OnSubscribeSignal = bs::signal<void(const int messageId, const MqttGrantedQoS&)>;
    using OnUnsubscribeSignal = bs::signal<void(const int messageId)>;
    using OnLogSignal = bs::signal<void(const MqttLogLevel, const std::string& message)>;

    IMqttClient() = default;

    virtual ~IMqttClient() = default;

    [[nodiscard]] virtual bool
    durable() const noexcept = 0;

    [[nodiscard]] virtual bool
    connected() const noexcept = 0;

    virtual void
    connect(const std::string& host, unsigned short port, int keepalive)
        = 0;

    virtual void
    disconnect()
        = 0;

    virtual void
    setCredentials(const std::string& username, const std::string& password)
        = 0;

    virtual void
    setReconnectDelay(unsigned int delay, unsigned int delayMax, bool exponentialBackoff)
        = 0;

    virtual void
    setInsecure(bool insecure)
        = 0;

    virtual void
    setTransportLayerVersion(MqttVerificationRequirements requirements,
                             MqttTransportLayerVersions version)
        = 0;

    virtual void
    configureTransportLayerSecurity(const std::string& path)
        = 0;

    virtual void
    configureTransportLayerSecurity(const std::string& psk,
                                    const std::string& identity,
                                    const std::string& ciphers)
        = 0;

    virtual void
    configureTransportLayerSecurity(const std::string& caFile,
                                    const std::string& certFile,
                                    const std::string& keyFile,
                                    std::optional<PasswordProviderCallback> passwordProvider)
        = 0;

    virtual void
    setProtocolVersion(MqttProtocolVersion version)
        = 0;

    virtual void
    willSet(
        const std::string& topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
        = 0;

    virtual void
    willSet(const std::string& topic, const std::string& message, MqttQoS qos, bool retain)
        = 0;

    virtual void
    willClear()
        = 0;

    virtual int
    publish(
        const std::string& topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
        = 0;

    virtual int
    publish(const std::string& topic, const std::string& message, MqttQoS qos, bool retain)
        = 0;

    virtual int
    subscribe(const std::string& topic, MqttQoS qos)
        = 0;

    virtual int
    unsubscribe(const std::string& topic)
        = 0;

    virtual bs::connection
    onConnect(const OnConnectSignal::slot_type& slot)
        = 0;

    virtual bs::connection
    onDisconnect(const OnDisconnectSignal::slot_type& slot)
        = 0;

    virtual bs::connection
    onPublish(const OnPublishSignal::slot_type& slot)
        = 0;

    virtual bs::connection
    onMessage(const OnMessageSignal::slot_type& slot)
        = 0;

    virtual bs::connection
    onSubscribe(const OnSubscribeSignal::slot_type& slot)
        = 0;

    virtual bs::connection
    onUnsubscribe(const OnUnsubscribeSignal::slot_type& slot)
        = 0;

    virtual bs::connection
    onLog(const OnLogSignal::slot_type& slot)
        = 0;

protected:
    IMqttClient(const IMqttClient&) = default;
    IMqttClient&
    operator=(const IMqttClient&)
        = default;
};

} // namespace connectivity
#pragma once

#include <gmock/gmock.h>

#include "connectivity/IMqttClient.hpp"

#include <memory>

namespace connectivity {

class MockMqttClient : public IMqttClient {
public:
    using Ptr = std::shared_ptr<MockMqttClient>;

    MockMqttClient();

    void
    triggerConnect();

    void
    triggerDisconnect(int reason);

    void
    triggerPublish(int messageId);

    void
    triggerMessage(const MqttMessage&);

    void
    triggerSubscribe(int messageId, const MqttGrantedQoS& grantedQos = {});

    void
    triggerUnsubscribe(int messageId);

    void
    triggerLog(MqttLogLevel level, const std::string& message);

    bs::connection
    onConnect(const OnConnectSignal::slot_type& slot) override;

    bs::connection
    onDisconnect(const OnDisconnectSignal::slot_type& slot) override;

    bs::connection
    onPublish(const OnPublishSignal::slot_type& slot) override;

    bs::connection
    onMessage(const OnMessageSignal::slot_type& slot) override;

    bs::connection
    onSubscribe(const OnSubscribeSignal::slot_type& slot) override;

    bs::connection
    onUnsubscribe(const OnUnsubscribeSignal::slot_type& slot) override;

    bs::connection
    onLog(const OnLogSignal::slot_type& slot) override;

public:
    MOCK_METHOD(bool, durable, (), (const, noexcept, override));

    MOCK_METHOD(bool, connected, (), (const, noexcept, override));

    MOCK_METHOD(void,
                connect,
                (const std::string& host, unsigned short port, int keepalive),
                (override));

    MOCK_METHOD(void, disconnect, (), (override));

    MOCK_METHOD(void,
                setCredentials,
                (const std::string& username, const std::string& password),
                (override));

    MOCK_METHOD(void,
                setReconnectDelay,
                (unsigned int delay, unsigned int delayMax, bool exponentialBackoff),
                (override));

    MOCK_METHOD(void, setInsecure, (bool insecure), (override));

    MOCK_METHOD(void,
                setTransportLayerVersion,
                (MqttVerificationRequirements, MqttTransportLayerVersions),
                (override));

    MOCK_METHOD(void, configureTransportLayerSecurity, (const std::string& path), (override));

    MOCK_METHOD(void,
                configureTransportLayerSecurity,
                (const std::string& psk, const std::string& identity, const std::string& ciphers),
                (override));

    MOCK_METHOD(void,
                configureTransportLayerSecurity,
                (const std::string& caFile,
                 const std::string& certFile,
                 const std::string& keyFile,
                 std::optional<PasswordProviderCallback>),
                (override));

    MOCK_METHOD(void, setProtocolVersion, (MqttProtocolVersion), (override));

    MOCK_METHOD(
        void,
        willSet,
        (const std::string& topic, const void* payload, std::size_t size, MqttQoS qos, bool retain),
        (override));

    MOCK_METHOD(void,
                willSet,
                (const std::string& topic, const std::string& message, MqttQoS qos, bool retain),
                (override));

    MOCK_METHOD(void, willClear, (), (override));

    MOCK_METHOD(
        int,
        publish,
        (const std::string& topic, const void* payload, std::size_t size, MqttQoS qos, bool retain),
        (override));

    MOCK_METHOD(int,
                publish,
                (const std::string& topic, const std::string& message, MqttQoS qos, bool retain),
                (override));

    MOCK_METHOD(int, subscribe, (const std::string& topic, MqttQoS qos), (override));

    MOCK_METHOD(int, unsubscribe, (const std::string& topic), (override));

private:
    OnConnectSignal _connectSig;
    OnDisconnectSignal _disconnectSig;
    OnPublishSignal _publishSig;
    OnMessageSignal _messageSig;
    OnSubscribeSignal _subscribeSig;
    OnUnsubscribeSignal _unsubscribeSig;
    OnLogSignal _logSig;
};

} // namespace connectivity
#pragma once

#include "connectivity/IMqttClient.hpp"

#include <atomic>

namespace bs = boost::signals2;

namespace connectivity {

class MqttClient : public IMqttClient {
public:
    explicit MqttClient(bool cleanSession = true);

    explicit MqttClient(const std::string& id, bool cleanSession = true);

    ~MqttClient() override;

    [[nodiscard]] bool
    durable() const noexcept override;

    [[nodiscard]] bool
    connected() const noexcept override;

    void
    connect(const std::string& host, unsigned short port, int keepalive) override;

    void
    disconnect() override;

    void
    setCredentials(const std::string& username, const std::string& password) override;

    void
    setReconnectDelay(unsigned int delay, unsigned int delayMax, bool exponentialBackoff) override;

    void
    setInsecure(bool insecure) override;

    void
    setTransportLayerVersion(MqttVerificationRequirements requirements,
                             MqttTransportLayerVersions version) override;

    void
    configureTransportLayerSecurity(const std::string& path) override;

    void
    configureTransportLayerSecurity(const std::string& psk,
                                    const std::string& identity,
                                    const std::string& ciphers) override;

    void
    configureTransportLayerSecurity(
        const std::string& caFile,
        const std::string& certFile,
        const std::string& keyFile,
        std::optional<PasswordProviderCallback> passwordProvider) override;

    void
    setProtocolVersion(MqttProtocolVersion version) override;

    void
    willSet(const std::string& topic,
            const void* payload,
            std::size_t size,
            MqttQoS qos,
            bool retain) override;

    void
    willSet(const std::string& topic,
            const std::string& message,
            MqttQoS qos,
            bool retain) override;

    void
    willClear() override;

    int
    publish(const std::string& topic,
            const void* payload,
            std::size_t size,
            MqttQoS qos,
            bool retain) override;

    int
    publish(const std::string& topic,
            const std::string& message,
            MqttQoS qos,
            bool retain) override;

    int
    subscribe(const std::string& topic, MqttQoS qos) override;

    int
    unsubscribe(const std::string& topic) override;

    [[nodiscard]] bs::connection
    onConnect(const OnConnectSignal::slot_type& slot) override;

    [[nodiscard]] bs::connection
    onDisconnect(const OnDisconnectSignal::slot_type& slot) override;

    [[nodiscard]] bs::connection
    onPublish(const OnPublishSignal::slot_type& slot) override;

    [[nodiscard]] bs::connection
    onMessage(const OnMessageSignal::slot_type& slot) override;

    [[nodiscard]] bs::connection
    onSubscribe(const OnSubscribeSignal::slot_type& slot) override;

    [[nodiscard]] bs::connection
    onUnsubscribe(const OnUnsubscribeSignal::slot_type& slot) override;

    [[nodiscard]] bs::connection
    onLog(const OnLogSignal::slot_type& slot) override;

private:
    static void
    onConnect(MqttContext* context, void* object, int status);

    static void
    onDisconnect(MqttContext* context, void* object, int reason);

    static void
    onPublish(MqttContext* context, void* object, int messageId);

    static void
    onMessage(MqttContext* context, void* object, const mosquitto_message* message);

    static void
    onSubscribe(MqttContext* context, void* object, int messageId, int qosCount, const int* qos);

    static void
    onUnsubscribe(MqttContext* context, void* object, int messageId);

    static void
    onLog(MqttContext* context, void* object, int level, const char* message);

private:
    void
    emitConnectSignal();

    void
    emitDisconnectSignal(int reason);

    void
    emitPublishSignal(int messageId);

    void
    emitMessageSignal(const MqttMessage& message);

    void
    emitSubscribeSignal(int messageId, const MqttGrantedQoS& qos);

    void
    emitUnsubscribeSignal(int messageId);

    void
    emitLogSignal(MqttLogLevel level, const std::string& message);

private:
    MqttContext* _context;

    OnConnectSignal _connectSig;
    OnDisconnectSignal _disconnectSig;
    OnPublishSignal _publishSig;
    OnMessageSignal _messageSig;
    OnSubscribeSignal _subscribeSig;
    OnUnsubscribeSignal _unsubscribeSig;
    OnLogSignal _logSig;

    std::atomic<bool> _durable;
    std::atomic<bool> _connected;
    std::atomic<bool> _active;
};

} // namespace connectivity
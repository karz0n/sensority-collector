#pragma once

#include "connectivity/IMqttClient.hpp"
#include "storage/DataWriter.hpp"

#include <boost/signals2/connection.hpp>

#include <string>
#include <memory>
#include <atomic>

namespace bs = boost::signals2;

namespace storage {

class DataObserver final : public std::enable_shared_from_this<DataObserver> {
public:
    using Ptr = std::shared_ptr<DataObserver>;

    static Ptr
    create(connectivity::IMqttClient::Ptr client, std::string topic, IDataWriter::Ptr dataWriter);

    [[nodiscard]] bool
    subscribed() const noexcept;

    void
    initialize();

    void
    uninitialize();

    void
    subscribe();

    void
    unsubscribe();

private:
    DataObserver(connectivity::IMqttClient::Ptr client,
                 std::string topic,
                 IDataWriter::Ptr dataWriter);

    void
    onConnect();

    void
    onDisconnect();

    void
    onSubscribe(int messageId);

    void
    onUnsubscribe(int messageId);

    void
    onMessage(const connectivity::MqttMessage& message);

private:
    connectivity::IMqttClient::Ptr _client;
    const std::string _topic;
    IDataWriter::Ptr _dataWriter;

    std::atomic<int> _messageId;
    std::atomic<bool> _needSubscribe;
    std::atomic<bool> _subscribed;

    bs::connection _onConnectCon;
    bs::connection _onDisconnectCon;
    bs::connection _onSubscribeCon;
    bs::connection _onUnsubscribeCon;
    bs::connection _onMessageCon;
};

} // namespace storage
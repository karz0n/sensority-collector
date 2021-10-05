#pragma once

#include "connectivity/IMqttClient.hpp"
#include "storage/IDataWriter.hpp"
#include "storage/IDataStorage.hpp"
#include "storage/DataObserver.hpp"

#include <boost/signals2/connection.hpp>

#include <string>
#include <memory>
#include <atomic>

namespace bs = boost::signals2;

namespace storage {

class SensorDataObserver final : public DataObserver,
                                 public std::enable_shared_from_this<SensorDataObserver> {
public:
    static Ptr
    create(connectivity::IMqttClient::Ptr client,
           IDataStorage::Ptr storage,
           std::string topic,
           IDataWriter::Ptr dataWriter);

    void
    initialize() override;

    void
    uninitialize() override;

    void
    subscribe() override;

    void
    unsubscribe() override;

private:
    SensorDataObserver(connectivity::IMqttClient::Ptr client,
                       IDataStorage::Ptr storage,
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
    IDataWriter::Ptr _writer;
    IDataStorage::Ptr _storage;

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
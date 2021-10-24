#pragma once

#include "connectivity/IMqttClient.hpp"
#include "storage/DataAccessor.hpp"
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
    create(connectivity::IMqttClient::Ptr client, std::string topic, DataAccessor::Ptr accessor);

    void
    setUp() override;

    void
    tearDown() override;

    void
    subscribe() override;

    void
    unsubscribe() override;

private:
    SensorDataObserver(connectivity::IMqttClient::Ptr client,
                       std::string topic,
                       DataAccessor::Ptr accessor);

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
    DataAccessor::Ptr _accessor;
    IDataStorage::Ptr _storage;

    std::atomic<int> _mid{connectivity::InvalidMessageId};
    std::atomic<bool> _subscribe{false};
    std::atomic<bool> _subscribed{false};

    bs::scoped_connection _onConnectCon;
    bs::scoped_connection _onDisconnectCon;
    bs::scoped_connection _onSubscribeCon;
    bs::scoped_connection _onUnsubscribeCon;
    bs::scoped_connection _onMessageCon;
};

} // namespace storage
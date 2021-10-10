#include "storage/SensorDataObserver.hpp"

#include "common/Logger.hpp"

using namespace connectivity;

namespace storage {

SensorDataObserver::Ptr
SensorDataObserver::create(connectivity::IMqttClient::Ptr client,
                           IDataStorage::Ptr storage,
                           std::string topic,
                           IDataWriter::Ptr writer)
{
    return std::shared_ptr<SensorDataObserver>(new SensorDataObserver{
        std::move(client), std::move(storage), std::move(topic), std::move(writer)});
}

SensorDataObserver::SensorDataObserver(IMqttClient::Ptr client,
                                       IDataStorage::Ptr storage,
                                       std::string topic,
                                       IDataWriter::Ptr writer)
    : _client{std::move(client)}
    , _topic{std::move(topic)}
    , _writer{std::move(writer)}
    , _storage{std::move(storage)}
{
}

void
SensorDataObserver::setUp()
{
    _onSubscribeCon = _client->onSubscribe([weakSelf = weak_from_this()](int mid, const auto&) {
        if (auto self = weakSelf.lock()) {
            self->onSubscribe(mid);
        }
    });

    _onUnsubscribeCon = _client->onUnsubscribe([weakSelf = weak_from_this()](const int mid) {
        if (auto self = weakSelf.lock()) {
            self->onUnsubscribe(mid);
        }
    });

    _onConnectCon = _client->onConnect([weakSelf = weak_from_this()]() {
        if (auto self = weakSelf.lock()) {
            self->onConnect();
        }
    });

    _onDisconnectCon = _client->onDisconnect([weakSelf = weak_from_this()](int) {
        if (auto self = weakSelf.lock()) {
            self->onDisconnect();
        }
    });

    _onMessageCon = _client->onMessage([weakSelf = weak_from_this()](const MqttMessage& message) {
        if (auto self = weakSelf.lock()) {
            self->onMessage(message);
        }
    });
}

void
SensorDataObserver::tearDown()
{
    try {
        _onSubscribeCon.disconnect();
        _onUnsubscribeCon.disconnect();
        _onConnectCon.disconnect();
        _onDisconnectCon.disconnect();
        _onMessageCon.disconnect();
    } catch (...) {
        // Suppress exceptions
    }
}

void
SensorDataObserver::subscribe()
{
    if (_client->connected()) {
        _mid = _client->subscribe(_topic, MqttQoS::v1);
    } else {
        _subscribe = true;
    }
}

void
SensorDataObserver::unsubscribe()
{
    if (_subscribed) {
        _mid = _client->unsubscribe(_topic);
    }
}

void
SensorDataObserver::onConnect()
{
    if (_subscribe) {
        subscribe();
    }
}

void
SensorDataObserver::onDisconnect()
{
    if (_subscribed && !_client->durable()) {
        _subscribe = true;
    }
}

void
SensorDataObserver::onSubscribe(const int messageId)
{
    if (messageId == _mid) {
        _subscribe = false;
        _subscribed = true;
        _mid = InvalidMessageId;
    }
}

void
SensorDataObserver::onUnsubscribe(const int messageId)
{
    if (messageId == _mid) {
        _subscribe = false;
        _subscribed = false;
        _mid = InvalidMessageId;
    }
}

void
SensorDataObserver::onMessage(const MqttMessage& message)
{
    if (message.topic == _topic) {
        const std::string input{reinterpret_cast<const char*>(message.payload.data()),
                                message.payload.size()};
        if (auto writer = _writer->clone(); writer->parse(input)) {
            _storage->process(std::move(writer));
        }
    }
}

} // namespace storage

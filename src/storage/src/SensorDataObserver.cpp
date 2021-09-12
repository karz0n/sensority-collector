#include "storage/SensorDataObserver.hpp"

#include "connectivity/MqttCommon.hpp"
#include "common/Logger.hpp"

#include <Poco/JSON/Parser.h>

using Poco::JSON::Parser;
using Poco::JSON::Object;

using namespace connectivity;

namespace {

Object::Ptr
toJsonObject(const std::string& data)
{
    static Parser parser;
    try {
        const auto var = parser.parse(data);
        if (!var.isEmpty()) {
            return var.extract<Object::Ptr>();
        }
        parser.reset();
    } catch (...) {
        // Suppress exceptions
    }
    return Object::Ptr{};
}

} // namespace

namespace storage {

SensorDataObserver::Ptr
SensorDataObserver::create(connectivity::IMqttClient::Ptr client,
                           std::string topic,
                           IDataWriter::Ptr dataWriter)
{
    return std::shared_ptr<SensorDataObserver>(
        new SensorDataObserver{std::move(client), std::move(topic), std::move(dataWriter)});
}

SensorDataObserver::SensorDataObserver(IMqttClient::Ptr client,
                                       std::string topic,
                                       IDataWriter::Ptr dataWriter)
    : _client{std::move(client)}
    , _topic{std::move(topic)}
    , _dataWriter{std::move(dataWriter)}
    , _messageId{InvalidMessageId}
    , _needSubscribe{false}
    , _subscribed{false}
{
}

void
SensorDataObserver::initialize()
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
SensorDataObserver::uninitialize()
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
        _messageId = _client->subscribe(_topic, MqttQoS::v1);
    } else {
        _needSubscribe = true;
    }
}

void
SensorDataObserver::unsubscribe()
{
    if (_subscribed) {
        _messageId = _client->unsubscribe(_topic);
    }
}

void
SensorDataObserver::onConnect()
{
    if (_needSubscribe) {
        subscribe();
    }
}

void
SensorDataObserver::onDisconnect()
{
    if (_subscribed && !_client->durable()) {
        _needSubscribe = true;
    }
}

void
SensorDataObserver::onSubscribe(const int messageId)
{
    if (messageId == _messageId) {
        _needSubscribe = false;
        _subscribed = true;
        _messageId = InvalidMessageId;
    }
}

void
SensorDataObserver::onUnsubscribe(const int messageId)
{
    if (messageId == _messageId) {
        _needSubscribe = false;
        _subscribed = false;
        _messageId = InvalidMessageId;
    }
}

void
SensorDataObserver::onMessage(const MqttMessage& message)
{
    if (message.topic == _topic) {
        const std::string data{reinterpret_cast<const char*>(message.payload.data()),
                               message.payload.size()};
        if (auto object = toJsonObject(data)) {
            _dataWriter->process(object);
        }
    }
}

} // namespace storage

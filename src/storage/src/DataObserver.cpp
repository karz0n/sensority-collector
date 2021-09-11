#include "storage/DataObserver.hpp"

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

DataObserver::Ptr
DataObserver::create(connectivity::IMqttClient::Ptr client,
                     std::string topic,
                     IDataWriter::Ptr dataWriter)
{
    return std::shared_ptr<DataObserver>(
        new DataObserver{std::move(client), std::move(topic), std::move(dataWriter)});
}

DataObserver::DataObserver(IMqttClient::Ptr client, std::string topic, IDataWriter::Ptr dataWriter)
    : _client{std::move(client)}
    , _topic{std::move(topic)}
    , _dataWriter{std::move(dataWriter)}
    , _messageId{InvalidMessageId}
    , _needSubscribe{false}
    , _subscribed{false}
{
}

bool
DataObserver::subscribed() const noexcept
{
    return _subscribed;
}

void
DataObserver::initialize()
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
DataObserver::uninitialize()
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
DataObserver::subscribe()
{
    if (_client->connected()) {
        _messageId = _client->subscribe(_topic, MqttQoS::v1);
    } else {
        _needSubscribe = true;
    }
}

void
DataObserver::unsubscribe()
{
    if (_subscribed) {
        _messageId = _client->unsubscribe(_topic);
    }
}

void
DataObserver::onConnect()
{
    if (_needSubscribe) {
        subscribe();
    }
}

void
DataObserver::onDisconnect()
{
    if (_subscribed && !_client->durable()) {
        _needSubscribe = true;
    }
}

void
DataObserver::onSubscribe(const int messageId)
{
    if (messageId == _messageId) {
        _needSubscribe = false;
        _subscribed = true;
        _messageId = InvalidMessageId;
    }
}

void
DataObserver::onUnsubscribe(const int messageId)
{
    if (messageId == _messageId) {
        _needSubscribe = false;
        _subscribed = false;
        _messageId = InvalidMessageId;
    }
}

void
DataObserver::onMessage(const MqttMessage& message)
{
    if (message.topic == _topic) {
        std::string data{reinterpret_cast<const char*>(message.payload.data()),
                         message.payload.size()};
        if (auto object = toJsonObject(data)) {
            _dataWriter->process(object);
        }
    }
}

} // namespace storage

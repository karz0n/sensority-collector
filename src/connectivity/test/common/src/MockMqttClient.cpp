#include "connectivity/MockMqttClient.hpp"

using namespace testing;

namespace connectivity {

MockMqttClient::MockMqttClient()
{
    ON_CALL(*this, durable).WillByDefault(Return(true));

    ON_CALL(*this, connected).WillByDefault(Return(false));

    ON_CALL(*this,
            publish(An<const std::string&>(),
                    An<const void*>(),
                    An<std::size_t>(),
                    An<MqttQoS>(),
                    An<bool>()))
        .WillByDefault(Return(InvalidMessageId));

    ON_CALL(*this,
            publish(An<const std::string&>(), An<const std::string&>(), An<MqttQoS>(), An<bool>()))
        .WillByDefault(Return(InvalidMessageId));

    ON_CALL(*this, subscribe).WillByDefault(Return(InvalidMessageId));

    ON_CALL(*this, unsubscribe).WillByDefault(Return(InvalidMessageId));
}

void
MockMqttClient::triggerConnect()
{
    _connectSig();
}

void
MockMqttClient::triggerDisconnect(const int reason)
{
    _disconnectSig(reason);
}

void
MockMqttClient::triggerPublish(const int messageId)
{
    _publishSig(messageId);
}

void
MockMqttClient::triggerMessage(const MqttMessage& message)
{
    _messageSig(message);
}

void
MockMqttClient::triggerSubscribe(const int messageId, const MqttGrantedQoS& grantedQos)
{
    _subscribeSig(messageId, grantedQos);
}

void
MockMqttClient::triggerUnsubscribe(const int messageId)
{
    _unsubscribeSig(messageId);
}

void
MockMqttClient::triggerLog(const MqttLogLevel level, const std::string& message)
{
    _logSig(level, message);
}

bs::connection
MockMqttClient::onConnect(const OnConnectSignal::slot_type& slot)
{
    return _connectSig.connect(slot);
}

bs::connection
MockMqttClient::onDisconnect(const OnDisconnectSignal::slot_type& slot)
{
    return _disconnectSig.connect(slot);
}

bs::connection
MockMqttClient::onPublish(const OnPublishSignal::slot_type& slot)
{
    return _publishSig.connect(slot);
}

bs::connection
MockMqttClient::onMessage(const OnMessageSignal::slot_type& slot)
{
    return _messageSig.connect(slot);
}

bs::connection
MockMqttClient::onSubscribe(const OnSubscribeSignal::slot_type& slot)
{
    return _subscribeSig.connect(slot);
}

bs::connection
MockMqttClient::onUnsubscribe(const OnUnsubscribeSignal::slot_type& slot)
{
    return _unsubscribeSig.connect(slot);
}

bs::connection
MockMqttClient::onLog(const OnLogSignal::slot_type& slot)
{
    return _logSig.connect(slot);
}

} // namespace connectivity
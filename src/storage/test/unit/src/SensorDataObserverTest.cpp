#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "connectivity/MockMqttClient.hpp"
#include "storage/MockDataAccessor.hpp"
#include "storage/MockDataStorage.hpp"

#include "storage/SensorDataObserver.hpp"

using namespace testing;
using namespace connectivity;
using namespace storage;

class SensorDataObserverTest : public Test {
public:
    static constexpr std::string_view TopicName{"temperature"};

    SensorDataObserverTest()
        : accessor{std::make_shared<NiceMock<MockDataAccessor>>()}
        , client{std::make_shared<NiceMock<MockMqttClient>>()}
        , observer{SensorDataObserver::create(client, std::string{TopicName}, accessor)}
    {
    }

    void
    SetUp() override
    {
        observer->setUp();
    }

    void
    TearDown() override
    {
        observer->tearDown();
    }

public:
    MockDataAccessor::Ptr accessor;
    MockMqttClient::Ptr client;
    SensorDataObserver::Ptr observer;
};

TEST_F(SensorDataObserverTest, SubscribeUnsubscribe)
{
    const int messageId = 1;

    EXPECT_CALL(*client, connected).WillOnce(Return(true));
    EXPECT_CALL(*client, subscribe).WillOnce(Return(messageId));
    observer->subscribe();
    client->triggerSubscribe(messageId);

    EXPECT_CALL(*client, unsubscribe).WillOnce(Return(messageId));
    observer->unsubscribe();
    client->triggerUnsubscribe(messageId);
}

TEST_F(SensorDataObserverTest, WriteData)
{
    const int messageId = 1;
    const std::string_view value{R"([{"value:": 25.1, "raw": 26.2 }])"};

    EXPECT_CALL(*client, connected).WillOnce(Return(true));
    EXPECT_CALL(*client, subscribe).WillOnce(Return(messageId));
    observer->subscribe();
    client->triggerSubscribe(messageId);

    MqttMessage message;
    message.id = 1;
    message.topic = TopicName;
    message.payload.assign(value.cbegin(), value.cend());

    EXPECT_CALL(*accessor, put);

    client->triggerMessage(message);
}

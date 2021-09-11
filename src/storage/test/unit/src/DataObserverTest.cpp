#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "connectivity/MockMqttClient.hpp"
#include "storage/MockDataWriter.hpp"

#include "storage/DataObserver.hpp"

using namespace testing;
using namespace connectivity;
using namespace storage;

class DataObserverTest : public Test {
public:
    static constexpr std::string_view TopicName{"temperature"};

    DataObserverTest()
        : writer{std::make_shared<NiceMock<MockDataWriter>>()}
        , client{std::make_shared<NiceMock<MockMqttClient>>()}
        , observer{DataObserver::create(client, std::string{TopicName}, writer)}
    {
    }

    void
    SetUp() override
    {
        observer->initialize();
    }

    void
    TearDown() override
    {
        observer->uninitialize();
    }

public:
    MockDataWriter::Ptr writer;
    MockMqttClient::Ptr client;
    DataObserver::Ptr observer;
};

TEST_F(DataObserverTest, SubscribeUnsubscribe)
{
    const int messageId = 1;

    EXPECT_CALL(*client, connected).WillOnce(Return(true));
    EXPECT_CALL(*client, subscribe).WillOnce(Return(messageId));
    observer->subscribe();
    client->triggerSubscribe(messageId);
    EXPECT_TRUE(observer->subscribed());

    EXPECT_CALL(*client, unsubscribe).WillOnce(Return(messageId));
    observer->unsubscribe();
    client->triggerUnsubscribe(messageId);
    EXPECT_FALSE(observer->subscribed());
}

TEST_F(DataObserverTest, WriteData)
{
    const int messageId = 1;
    const std::string_view value{R"({"value:": 25.1, "raw": 26.2 })"};

    EXPECT_CALL(*client, connected).WillOnce(Return(true));
    EXPECT_CALL(*client, subscribe).WillOnce(Return(messageId));
    observer->subscribe();
    client->triggerSubscribe(messageId);
    ASSERT_TRUE(observer->subscribed());

    MqttMessage message;
    message.id = 1;
    message.topic = TopicName;
    message.payload.assign(value.cbegin(), value.cend());

    EXPECT_CALL(*writer, process(NotNull()));
    client->triggerMessage(message);
}

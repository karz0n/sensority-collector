#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "connectivity/MockMqttClient.hpp"
#include "storage/MockDataWriter.hpp"
#include "storage/MockDataStorage.hpp"

#include "storage/SensorDataObserver.hpp"

using namespace testing;
using namespace connectivity;
using namespace storage;

class SensorDataObserverTest : public Test {
public:
    static constexpr std::string_view TopicName{"temperature"};

    SensorDataObserverTest()
        : writer{std::make_shared<NiceMock<MockDataWriter>>()}
        , client{std::make_shared<NiceMock<MockMqttClient>>()}
        , storage{std::make_shared<NiceMock<MockDataStorage>>()}
        , observer{SensorDataObserver::create(client, storage, std::string{TopicName}, writer)}
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
    MockDataStorage::Ptr storage;
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
    const std::string_view value{R"({"value:": 25.1, "raw": 26.2 })"};

    EXPECT_CALL(*client, connected).WillOnce(Return(true));
    EXPECT_CALL(*client, subscribe).WillOnce(Return(messageId));
    observer->subscribe();
    client->triggerSubscribe(messageId);

    MqttMessage message;
    message.id = 1;
    message.topic = TopicName;
    message.payload.assign(value.cbegin(), value.cend());

    MockDataWriter::Ptr writerClone = std::make_shared<MockDataWriter>();
    EXPECT_CALL(*writerClone, parse);
    EXPECT_CALL(*writer, clone).WillOnce(Return(writerClone));

    client->triggerMessage(message);
}

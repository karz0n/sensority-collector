#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/TemperatureData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class TemperatureDataTest : public Test {
public:
    const std::string Value{R"({"value": 13.5, "raw": 14.7})"};

    void
    SetUp() override
    {
        storage.initialize();
    }

    void
    TearDown() override
    {
        storage.uninitialize();
    }

    static void
    SetUpTestSuite()
    {
        DataStorage::setUp();
    }

    static void
    TearDownTestSuite()
    {
        DataStorage::tearDown();
    }

public:
    DataStorage storage;
};

static Matcher<TemperatureData>
matchTo(float value, float raw)
{
    return AllOf(Field(&TemperatureData::value, FloatEq(value)),
                 Field(&TemperatureData::raw, FloatEq(raw)));
}

TEST_F(TemperatureDataTest, Parse)
{
    EXPECT_NO_THROW({
        TemperatureDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(TemperatureDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<TemperatureDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<TemperatureDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(13.5f, 14.7f)));
    });
}

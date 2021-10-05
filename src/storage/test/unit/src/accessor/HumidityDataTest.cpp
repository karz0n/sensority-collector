#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/HumidityData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class HumidityDataTest : public Test {
public:
    const std::string Value{R"({"value": 56.5, "raw": 57.7})"};

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

static Matcher<HumidityData>
matchTo(float value, float raw)
{
    return AllOf(Field(&HumidityData::value, FloatEq(value)),
                 Field(&HumidityData::raw, FloatEq(raw)));
}

TEST_F(HumidityDataTest, Parse)
{
    EXPECT_NO_THROW({
        HumidityDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(HumidityDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<HumidityDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<HumidityDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(56.5f, 57.7f)));
    });
}
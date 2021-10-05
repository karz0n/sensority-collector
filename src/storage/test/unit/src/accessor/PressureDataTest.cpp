#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/PressureData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class PressureDataTest : public Test {
public:
    const std::string Value{R"({"value": 10.5})"};

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

static Matcher<PressureData>
matchTo(float value)
{
    return AllOf(Field(&PressureData::value, FloatEq(value)));
}

TEST_F(PressureDataTest, Parse)
{
    EXPECT_NO_THROW({
        PressureDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(PressureDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<PressureDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<PressureDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(10.5f)));
    });
}
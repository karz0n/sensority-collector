#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/GasData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class GasDataTest : public Test {
public:
    const std::string Value{R"({"value": 15.5})"};

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

static Matcher<GasData>
matchTo(float value)
{
    return AllOf(Field(&GasData::value, FloatEq(value)));
}

TEST_F(GasDataTest, Parse)
{
    EXPECT_NO_THROW({
        GasDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(GasDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<GasDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<GasDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(15.5f)));
    });
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/TvocData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class TvocDataTest : public Test {
public:
    const std::string Value{R"({"value": 13.5})"};

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

static Matcher<TvocData>
matchTo(float value)
{
    return AllOf(Field(&TvocData::value, FloatEq(value)));
}

TEST_F(TvocDataTest, Parse)
{
    EXPECT_NO_THROW({
        TvocDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(TvocDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<TvocDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<TvocDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(13.5f)));
    });
}

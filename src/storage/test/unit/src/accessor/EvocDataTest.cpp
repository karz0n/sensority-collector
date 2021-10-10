#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/EvocData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class EvocDataTest : public Test {
public:
    const std::string Value{R"({"value": 25.5, "accuracy": 0.01, "equivalent": 50.0})"};

    void
    SetUp() override
    {
        storage.setUp();
    }

    void
    TearDown() override
    {
        storage.tearDown();
    }

    static void
    SetUpTestSuite()
    {
        DataStorage::initialize();
    }

    static void
    TearDownTestSuite()
    {
        DataStorage::uninitialize();
    }

public:
    DataStorage storage;
};

static Matcher<EvocData>
matchTo(float value, float accuracy, float equivalent)
{
    return AllOf(Field(&EvocData::value, FloatEq(value)),
                 Field(&EvocData::accuracy, FloatEq(accuracy)),
                 Field(&EvocData::equivalent, FloatEq(equivalent)));
}

TEST_F(EvocDataTest, Parse)
{
    EXPECT_NO_THROW({
        EvocDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(EvocDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<EvocDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<EvocDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(25.5f, 0.01f, 50.0f)));
    });
}
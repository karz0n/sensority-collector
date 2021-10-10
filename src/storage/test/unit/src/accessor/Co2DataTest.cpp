#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/Co2Data.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class Co2DataTest : public Test {
public:
    const std::string Value{R"({"value": 13.5, "accuracy": 0.1, "equivalent": 100.0})"};

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

static Matcher<Co2Data>
matchTo(float value, float accuracy, float equivalent)
{
    return AllOf(Field(&Co2Data::value, FloatEq(value)),
                 Field(&Co2Data::accuracy, FloatEq(accuracy)),
                 Field(&Co2Data::equivalent, FloatEq(equivalent)));
}

TEST_F(Co2DataTest, Parse)
{
    EXPECT_NO_THROW({
        Co2DataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(Co2DataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<Co2DataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<Co2DataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(13.5f, 0.1f, 100.0f)));
    });
}
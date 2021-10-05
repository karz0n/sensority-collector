#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/accessor/IaqData.hpp"
#include "storage/DataStorage.hpp"

using namespace testing;
using namespace storage;

class IaqDataTest : public Test {
public:
    const std::string Value{R"({"value": 10.5, "raw": 10.7, "accuracy": 0.1})"};

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

static Matcher<IaqData>
matchTo(float value, float raw, float accuracy)
{
    return AllOf(Field(&IaqData::value, FloatEq(value)),
                 Field(&IaqData::raw, FloatEq(raw)),
                 Field(&IaqData::accuracy, FloatEq(accuracy)));
}

TEST_F(IaqDataTest, Parse)
{
    EXPECT_NO_THROW({
        IaqDataWriter writer;
        writer.parse(Value);
    });
}

TEST_F(IaqDataTest, Store)
{
    EXPECT_NO_THROW({
        auto writer = std::make_shared<IaqDataWriter>();
        writer->parse(Value);
        auto f = storage.process(writer);
        ASSERT_TRUE(f.valid());
        f.wait();
    });

    EXPECT_NO_THROW({
        auto reader = std::make_shared<IaqDataReader>();
        auto f = storage.process(reader);
        ASSERT_TRUE(f.valid());
        f.wait();
        ASSERT_THAT(reader->records(), Not(IsEmpty()));
        EXPECT_THAT(reader->records(), Contains(matchTo(10.5f, 10.7f, 0.1f)));
    });
}
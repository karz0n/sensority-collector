#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/eCo2DataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<eCo2Data>
matchTo(float value, float accuracy, float equivalent)
{
    return AllOf(Field(&eCo2Data::equivalent, FloatEq(equivalent)),
                 Field(&eCo2Data::accuracy, FloatEq(accuracy)));
}

TEST(Co2ModelTest, Parse)
{
    const std::string Values{R"([
        {"value": 13.5, "accuracy": 0.01, "equivalent": 1000.0},
        {"value": 10.5, "accuracy": 0.02, "equivalent": 1500.0}
    ])"};

    eCo2DataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 0.01, 1000.0), matchTo(10.5, 0.02, 1500.0)));
}
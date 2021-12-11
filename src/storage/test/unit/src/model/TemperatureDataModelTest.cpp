#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/TemperatureDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<TemperatureData>
matchTo(float value, float raw)
{
    return AllOf(Field(&TemperatureData::value, FloatEq(value)),
                 Field(&TemperatureData::raw, FloatEq(raw)));
}

TEST(TemperatureDataModelTest, Parse)
{
    const std::string Values{R"([
        {"value": 13.5, "raw": 14.7},
        {"value": 10.1, "raw": 11.2}
    ])"};

    TemperatureDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 14.7), matchTo(10.1, 11.2)));
}

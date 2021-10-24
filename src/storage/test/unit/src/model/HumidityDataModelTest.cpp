#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/HumidityDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<HumidityData>
matchTo(float value, float raw)
{
    return AllOf(Field(&HumidityData::value, FloatEq(value)),
                 Field(&HumidityData::raw, FloatEq(raw)));
}

TEST(HumidityDataModelTest, Parse)
{
    const std::string Values{R"([
        {"value": 11.5, "raw": 12.7},
        {"value": 10.1, "raw": 11.2}
    ])"};

    HumidityDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(11.5, 12.7), matchTo(10.1, 11.2)));
}
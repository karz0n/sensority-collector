#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/GasDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<GasData>
matchTo(float value, float percentage, float percentageAccuracy)
{
    return AllOf(Field(&GasData::value, FloatEq(value)),
                 Field(&GasData::percentage, FloatEq(percentage)),
                 Field(&GasData::percentageAccuracy, FloatEq(percentageAccuracy)));
}

TEST(GasDataModelTest, Parse)
{
    const std::string Values{R"([
        {"value": 13.5, "percentage": 21.1, "percentageAccuracy": 0.01},
        {"value": 15.5, "percentage": 22.0, "percentageAccuracy": 0.02}
    ])"};

    GasDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 21.1, 0.01), matchTo(15.5, 22.0, 0.02)));
}
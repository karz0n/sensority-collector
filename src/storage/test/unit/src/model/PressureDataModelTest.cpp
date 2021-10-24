#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/PressureDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<PressureData>
matchTo(float value)
{
    return AllOf(Field(&PressureData::value, FloatEq(value)));
}

TEST(PressureDataModelTest, Parse)
{
    const std::string Values{R"([
        {"value": 13.5},
        {"value": 10.1}
    ])"};

    PressureDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(13.5), matchTo(10.1)));
}

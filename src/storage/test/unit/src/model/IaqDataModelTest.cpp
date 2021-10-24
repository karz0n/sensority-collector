#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/IaqDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<IaqData>
matchTo(float value, float accuracy, float staticValue, float staticValueAccuracy)
{
    return AllOf(Field(&IaqData::value, FloatEq(value)),
                 Field(&IaqData::accuracy, FloatEq(accuracy)),
                 Field(&IaqData::staticValue, FloatEq(staticValue)),
                 Field(&IaqData::staticValueAccuracy, FloatEq(staticValueAccuracy)));
}

TEST(IaqDataModelTest, Parse)
{
    const std::string Values{R"([
        {"value": 13.5, "accuracy": 0.1, "staticValue": 10.1, "staticValueAccuracy": 0.1},
        {"value": 11.5, "accuracy": 0.2, "staticValue": 11.1, "staticValueAccuracy": 0.2}
    ])"};

    IaqDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 0.1, 10.1, 0.1), matchTo(11.5, 0.2, 11.1, 0.2)));
}
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/eBreathVocDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<eBreathVocData>
matchTo(float equivalent, float accuracy)
{
    return AllOf(Field(&eBreathVocData::equivalent, FloatEq(equivalent)),
                 Field(&eBreathVocData::accuracy, FloatEq(accuracy)));
}

TEST(eBreathVocDataModelTest, Parse)
{
    const std::string Values{R"([
        {"equivalent": 150.0, "accuracy": 0.01},
        {"equivalent": 200.0, "accuracy": 0.02}
    ])"};

    eBreathVocDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(150.0, 0.01), matchTo(200.0, 0.02)));
}
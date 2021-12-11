#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/model/BreathVocDataModel.hpp"

using namespace testing;
using namespace storage;

static Matcher<BreathVocData>
matchTo(float equivalent, float accuracy)
{
    return AllOf(Field(&BreathVocData::equivalent, FloatEq(equivalent)),
                 Field(&BreathVocData::accuracy, FloatEq(accuracy)));
}

TEST(BreathVocDataModelTest, Parse)
{
    const std::string Values{R"([
        {"equivalent": 150.0, "accuracy": 0.01},
        {"equivalent": 200.0, "accuracy": 0.02}
    ])"};

    BreathVocDataModel model;
    EXPECT_NO_THROW({ ASSERT_TRUE(model.parse(Values)); });
    EXPECT_THAT(model, SizeIs(2));
    EXPECT_THAT(model, ElementsAre(matchTo(150.0, 0.01), matchTo(200.0, 0.02)));
}
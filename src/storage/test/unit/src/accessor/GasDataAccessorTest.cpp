#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/model/GasDataModel.hpp"
#include "storage/accessor/GasDataAccessor.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class GasDataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"value": 13.5, "percentage": 10.0, "percentageAccuracy": 0.01},
        {"value": 12.5, "percentage": 11.0, "percentageAccuracy": 0.02}
    ])"};

    GasDataAccessorTest()
        : storage{std::make_shared<DataStorage>()}
        , accessor{storage}
    {
    }

    void
    SetUp() override
    {
        storage->setUp();
    }

    void
    TearDown() override
    {
        storage->tearDown();
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
    DataStorage::Ptr storage;
    GasDataAccessor accessor;
};

static Matcher<GasData>
matchTo(float value, float percentage, float percentageAccuracy)
{
    return AllOf(Field(&GasData::value, FloatEq(value)),
                 Field(&GasData::percentage, FloatEq(percentage)),
                 Field(&GasData::percentageAccuracy, FloatEq(percentageAccuracy)));
}

TEST_F(GasDataAccessorTest, Store)
{
    Waiter waiter;

    MockFunction<DataAccessor::PutCallbackSignature> putCallback;
    EXPECT_CALL(putCallback, Call(IsTrue()));
    bool safeGuard{false};
    accessor.put(Values, [&](const bool success) {
        putCallback.Call(success);
        safeGuard = true;
        waiter.notifyOne();
    });
    ASSERT_TRUE(waiter.waitFor(3s, [&]() { return safeGuard; }));

    MockFunction<DataAccessor::GetCallbackSignature> getCallback;
    EXPECT_CALL(getCallback, Call(Not(IsNull()), IsTrue()));
    IDataModel::Ptr dataModel;
    safeGuard = false;
    accessor.getForLastDay([&](IDataModel::Ptr model, const bool success) {
        getCallback.Call(dataModel = std::move(model), success);
        safeGuard = true;
        waiter.notifyOne();
    });
    ASSERT_TRUE(waiter.waitFor(3s, [&]() { return safeGuard; }));

    ASSERT_TRUE(dataModel->typeIs<GasDataModel>());
    const auto& model = dataModel->castTo<const GasDataModel>();
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 10.0, 0.01), matchTo(12.5, 11.0, 0.02)));
}

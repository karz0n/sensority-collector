#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/model/TemperatureDataModel.hpp"
#include "storage/accessor/TemperatureDataAccessor.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class TemperatureDataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"value": 13.5, "raw": 14.7},
        {"value": 11.5, "raw": 11.7}
    ])"};

    TemperatureDataAccessorTest()
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
    TemperatureDataAccessor accessor;
};

static Matcher<TemperatureData>
matchTo(float value, float raw)
{
    return AllOf(Field(&TemperatureData::value, FloatEq(value)), Field(&TemperatureData::raw, FloatEq(raw)));
}

TEST_F(TemperatureDataAccessorTest, Store)
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

    ASSERT_TRUE(dataModel->typeIs<TemperatureDataModel>());
    const auto& model = dataModel->castTo<const TemperatureDataModel>();
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 14.7), matchTo(11.5, 11.7)));
}

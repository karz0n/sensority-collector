#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/model/HumidityDataModel.hpp"
#include "storage/accessor/HumidityDataAccessor.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class HumidityDataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"value": 13.5, "raw": 14.7},
        {"value": 11.5, "raw": 11.7}
    ])"};

    HumidityDataAccessorTest()
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
    HumidityDataAccessor accessor;
};

static Matcher<HumidityData>
matchTo(float value, float raw)
{
    return AllOf(Field(&HumidityData::value, FloatEq(value)),
                 Field(&HumidityData::raw, FloatEq(raw)));
}

TEST_F(HumidityDataAccessorTest, Store)
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

    ASSERT_TRUE(dataModel->typeIs<HumidityDataModel>());
    const auto& model = dataModel->castTo<const HumidityDataModel>();
    EXPECT_THAT(model, ElementsAre(matchTo(13.5, 14.7), matchTo(11.5, 11.7)));
}
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/model/PressureDataModel.hpp"
#include "storage/accessor/PressureDataAccessor.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class PressureDataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"value": 13.5},
        {"value": 11.5}
    ])"};

    PressureDataAccessorTest()
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
    PressureDataAccessor accessor;
};

static Matcher<PressureData>
matchTo(float value)
{
    return AllOf(Field(&PressureData::value, FloatEq(value)));
}

TEST_F(PressureDataAccessorTest, Store)
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

    ASSERT_TRUE(dataModel->typeIs<PressureDataModel>());
    const auto& model = dataModel->castTo<const PressureDataModel>();
    EXPECT_THAT(model, ElementsAre(matchTo(13.5), matchTo(11.5)));
}
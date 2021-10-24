#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/model/IaqDataModel.hpp"
#include "storage/accessor/IaqDataAccessor.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class IaqDataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"value": 100.0, "accuracy": 0.01, "staticValue": 120.0, "staticValueAccuracy": 0.01},
        {"value": 110.0, "accuracy": 0.02, "staticValue": 130.0, "staticValueAccuracy": 0.02}
    ])"};

    IaqDataAccessorTest()
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
    IaqDataAccessor accessor;
};

static Matcher<IaqData>
matchTo(float value, float accuracy, float staticValue, float staticValueAccuracy)
{
    return AllOf(Field(&IaqData::value, FloatEq(value)),
                 Field(&IaqData::accuracy, FloatEq(accuracy)),
                 Field(&IaqData::staticValue, FloatEq(staticValue)),
                 Field(&IaqData::staticValueAccuracy, FloatEq(staticValueAccuracy)));
}

TEST_F(IaqDataAccessorTest, Store)
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

    ASSERT_TRUE(dataModel->typeIs<IaqDataModel>());
    const auto& model = dataModel->castTo<const IaqDataModel>();
    EXPECT_THAT(model,
                ElementsAre(matchTo(100.0, 0.01, 120.0, 0.01), matchTo(110.0, 0.02, 130.0, 0.02)));
}

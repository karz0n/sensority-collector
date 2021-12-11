#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/accessor/Co2DataAccessor.hpp"
#include "storage/model/Co2DataModel.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class Co2DataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"equivalent": 120.0, "accuracy": 0.01},
        {"equivalent": 130.0, "accuracy": 0.02}
    ])"};

    Co2DataAccessorTest()
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
    Co2DataAccessor accessor;
};

static Matcher<Co2Data>
matchTo(float equivalent, float accuracy)
{
    return AllOf(Field(&Co2Data::equivalent, FloatEq(equivalent)),
                 Field(&Co2Data::accuracy, FloatEq(accuracy)));
}

TEST_F(Co2DataAccessorTest, Store)
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

    ASSERT_TRUE(dataModel->typeIs<Co2DataModel>());
    const auto& model = dataModel->castTo<const Co2DataModel>();
    EXPECT_THAT(model, ElementsAre(matchTo(120.0, 0.01), matchTo(130.0, 0.02)));
}
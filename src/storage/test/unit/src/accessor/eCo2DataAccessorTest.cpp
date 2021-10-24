#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "storage/DataStorage.hpp"
#include "storage/accessor/eCo2DataAccessor.hpp"
#include "storage/model/eCo2DataModel.hpp"
#include "tests/Waiter.hpp"

using namespace testing;
using namespace storage;

using namespace std::chrono_literals;

class eCo2DataAccessorTest : public Test {
public:
    const std::string Values{R"([
        {"equivalent": 120.0, "accuracy": 0.01},
        {"equivalent": 130.0, "accuracy": 0.02}
    ])"};

    eCo2DataAccessorTest()
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
    eCo2DataAccessor accessor;
};

static Matcher<eCo2Data>
matchTo(float equivalent, float accuracy)
{
    return AllOf(Field(&eCo2Data::equivalent, FloatEq(equivalent)),
                 Field(&eCo2Data::accuracy, FloatEq(accuracy)));
}

TEST_F(eCo2DataAccessorTest, Store)
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

    ASSERT_TRUE(dataModel->typeIs<eCo2DataModel>());
    const auto& model = dataModel->castTo<const eCo2DataModel>();
    EXPECT_THAT(model, ElementsAre(matchTo(120.0, 0.01), matchTo(130.0, 0.02)));
}
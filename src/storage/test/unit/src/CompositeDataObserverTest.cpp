#include <gtest/gtest.h>

#include "storage/CompositeDataObserver.hpp"
#include "storage/MockDataObserver.hpp"

using namespace testing;
using namespace storage;

class CompositeDataObserverTest : public Test {
public:
    CompositeDataObserverTest()
        : tailObserver1{std::make_shared<MockDataObserver>()}
        , tailObserver2{std::make_shared<MockDataObserver>()}
        , tailObserver3{std::make_shared<MockDataObserver>()}
    {
        observer.add(tailObserver1);
        observer.add(tailObserver2);
        observer.add(tailObserver3);
    }

public:
    MockDataObserver::Ptr tailObserver1;
    MockDataObserver::Ptr tailObserver2;
    MockDataObserver::Ptr tailObserver3;
    CompositeDataObserver observer;
};

TEST_F(CompositeDataObserverTest, Initialize)
{
    EXPECT_CALL(*tailObserver1, initialize);
    EXPECT_CALL(*tailObserver2, initialize);
    EXPECT_CALL(*tailObserver3, initialize);

    observer.initialize();
}

TEST_F(CompositeDataObserverTest, Uninitialize)
{
    EXPECT_CALL(*tailObserver1, uninitialize);
    EXPECT_CALL(*tailObserver2, uninitialize);
    EXPECT_CALL(*tailObserver3, uninitialize);

    observer.uninitialize();
}

TEST_F(CompositeDataObserverTest, Subscribe)
{
    EXPECT_CALL(*tailObserver1, subscribe);
    EXPECT_CALL(*tailObserver2, subscribe);
    EXPECT_CALL(*tailObserver3, subscribe);

    observer.subscribe();
}

TEST_F(CompositeDataObserverTest, Unsubscribe)
{
    EXPECT_CALL(*tailObserver1, unsubscribe);
    EXPECT_CALL(*tailObserver2, unsubscribe);
    EXPECT_CALL(*tailObserver3, unsubscribe);

    observer.unsubscribe();
}

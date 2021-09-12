#pragma once

#include "storage/DataObserver.hpp"

#include <gmock/gmock.h>

#include <memory>

namespace storage {

class MockDataObserver : public DataObserver {
public:
    using Ptr = std::shared_ptr<MockDataObserver>;

    MOCK_METHOD(void, initialize, (), (override));

    MOCK_METHOD(void, uninitialize, (), (override));

    MOCK_METHOD(void, subscribe, (), (override));

    MOCK_METHOD(void, unsubscribe, (), (override));
};

} // namespace storage
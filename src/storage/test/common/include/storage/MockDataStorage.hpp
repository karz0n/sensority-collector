#pragma once

#include "storage/IDataStorage.hpp"

#include <gmock/gmock.h>

#include <memory>

namespace storage {

class MockDataStorage : public IDataStorage {
public:
    using Ptr = std::shared_ptr<MockDataStorage>;

    MOCK_METHOD(void, setUp, (), (override));

    MOCK_METHOD(void, tearDown, (), (override));

    MOCK_METHOD(std::future<bool>, process, (IDataAccessor::Ptr), (override));
};

} // namespace storage
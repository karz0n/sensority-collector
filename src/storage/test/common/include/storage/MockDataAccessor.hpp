#pragma once

#include "storage/IDataAccessor.hpp"

#include <gmock/gmock.h>

#include <memory>

namespace storage {

class MockDataAccessor : public IDataAccessor {
public:
    using Ptr = std::shared_ptr<MockDataAccessor>;

    MOCK_METHOD(void, put, (const std::string& input, PutCallback), (override));

    MOCK_METHOD(void, get, (int64_t from, int64_t to, GetCallback), (override));

    MOCK_METHOD(void, getForLastDay, (GetCallback), (override));

    MOCK_METHOD(void, getForLastWeek, (GetCallback), (override));

    MOCK_METHOD(void, getForLastMonth, (GetCallback), (override));
};

} // namespace storage
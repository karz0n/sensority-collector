#pragma once

#include "storage/DataAccessor.hpp"

#include <gmock/gmock.h>

#include <memory>

namespace storage {

class MockDataAccessor : public DataAccessor {
public:
    using Ptr = std::shared_ptr<MockDataAccessor>;

    MOCK_METHOD(void, put, (const std::string& input, PutCallback), (override));

    MOCK_METHOD(void, get, (int64_t from, int64_t to, GetCallback), (override));
};

} // namespace storage
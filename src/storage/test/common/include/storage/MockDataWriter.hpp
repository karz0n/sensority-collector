#pragma once

#include "storage/DataWriter.hpp"

#include <gmock/gmock.h>

#include <memory>

namespace storage {

class MockDataWriter : public IDataWriter {
public:
    using Ptr = std::shared_ptr<MockDataWriter>;

    MOCK_METHOD(void, process, (Poco::JSON::Object::Ptr), (override));
};

} // namespace storage
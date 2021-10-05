#pragma once

#include "storage/IDataWriter.hpp"

#include <gmock/gmock.h>

#include <memory>

namespace storage {

class MockDataWriter : public IDataWriter {
public:
    using Ptr = std::shared_ptr<MockDataWriter>;

    MOCK_METHOD(bool, parse, (const std::string& input), (override));

    MOCK_METHOD(void, access, (Poco::Data::Session&), (override));

    MOCK_METHOD(IDataWriter::Ptr, clone, (), (override));
};

} // namespace storage
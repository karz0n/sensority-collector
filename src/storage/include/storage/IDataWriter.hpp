#pragma once

#include "storage/IDataAccessor.hpp"

#include <memory>
#include <string>

namespace storage {

class IDataWriter : public IDataAccessor {
public:
    using Ptr = std::shared_ptr<IDataWriter>;

    virtual bool
    parse(const std::string& input)
        = 0;

    virtual Ptr
    clone()
        = 0;
};

} // namespace storage
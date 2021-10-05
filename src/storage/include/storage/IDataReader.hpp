#pragma once

#include "storage/IDataAccessor.hpp"

#include <memory>
#include <string>

namespace storage {

class IDataReader : public IDataAccessor {
public:
    using Ptr = std::shared_ptr<IDataReader>;

    virtual std::string
    stringify()
        = 0;

    virtual Ptr
    clone()
        = 0;
};

} // namespace storage
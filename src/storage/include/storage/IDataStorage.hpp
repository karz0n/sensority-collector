#pragma once

#include "storage/IDataAccessor.hpp"

#include <memory>
#include <future>

namespace storage {

class IDataStorage {
public:
    using Ptr = std::shared_ptr<IDataStorage>;

    IDataStorage() = default;

    virtual ~IDataStorage() = default;

    virtual void
    initialize()
        = 0;

    virtual void
    uninitialize()
        = 0;

    virtual std::future<bool>
    process(IDataAccessor::Ptr accessor) = 0;

protected:
    IDataStorage(const IDataStorage&) = default;
    IDataStorage&
    operator=(const IDataStorage&)
        = default;
};

} // namespace storage
#pragma once

#include "storage/IDataJob.hpp"

#include <memory>
#include <future>

namespace storage {

class IDataStorage {
public:
    using Ptr = std::shared_ptr<IDataStorage>;

    IDataStorage() = default;

    virtual ~IDataStorage() = default;

    virtual void
    setUp()
        = 0;

    virtual void
    tearDown()
        = 0;

    virtual void
    process(IDataJob::Ptr job)
        = 0;

protected:
    IDataStorage(const IDataStorage&) = default;
    IDataStorage&
    operator=(const IDataStorage&)
        = default;
};

} // namespace storage
#pragma once

#include <memory>
#include <string>

namespace storage {

class DataObserver {
public:
    using Ptr = std::shared_ptr<DataObserver>;

    DataObserver() = default;

    virtual ~DataObserver() = default;

    virtual void
    setUp()
        = 0;

    virtual void
    tearDown()
        = 0;

    virtual void
    subscribe()
        = 0;

    virtual void
    unsubscribe()
        = 0;

    virtual void
    add(DataObserver::Ptr observer);

    virtual void
    remove(DataObserver::Ptr observer);

protected:
    DataObserver(const DataObserver&) = default;
    DataObserver&
    operator=(const DataObserver&)
        = default;
};

} // namespace storage
#pragma once

#include "storage/DataObserver.hpp"

#include <set>
#include <memory>

namespace storage {

class CompositeDataObserver : public DataObserver {
public:
    void
    initialize() override;

    void
    uninitialize() override;

    void
    subscribe() override;

    void
    unsubscribe() override;

    void
    add(DataObserver::Ptr observer) override;

    void
    remove(DataObserver::Ptr observer) override;

private:
    std::set<DataObserver::Ptr, std::owner_less<DataObserver::Ptr>> _observers;
};

} // namespace storage
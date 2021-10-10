#include "storage/CompositeDataObserver.hpp"

namespace storage {

void
CompositeDataObserver::setUp()
{
    for (auto&& observer : _observers) {
        observer->setUp();
    }
}

void
CompositeDataObserver::tearDown()
{
    for (auto&& observer : _observers) {
        observer->tearDown();
    }
}

void
CompositeDataObserver::subscribe()
{
    for (auto&& observer : _observers) {
        observer->subscribe();
    }
}

void
CompositeDataObserver::unsubscribe()
{
    for (auto&& observer : _observers) {
        observer->unsubscribe();
    }
}

void
CompositeDataObserver::add(DataObserver::Ptr observer)
{
    _observers.insert(std::move(observer));
}

void
CompositeDataObserver::remove(DataObserver::Ptr observer)
{
    _observers.erase(observer);
}

} // namespace storage
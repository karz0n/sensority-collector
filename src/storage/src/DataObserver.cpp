#include "storage/DataObserver.hpp"

#include <Poco/Exception.h>

namespace storage {

void DataObserver::add(DataObserver::Ptr /*observer*/)
{
    throw Poco::LogicException{"Method not supported"};
}

void DataObserver::remove(DataObserver::Ptr /*observer*/)
{
    throw Poco::LogicException{"Method not supported"};
}

} // namespace storage

#include "storage/DataAccessor.hpp"

#include <Poco/DateTime.h>

namespace storage {

void
DataAccessor::getForLastDay(GetCallback callback)
{
    const Poco::DateTime currentDateTime;
    const auto minusDay = currentDateTime - Poco::Timespan{1, 0, 0, 0, 0};
    get(minusDay.timestamp().epochMicroseconds(),
        currentDateTime.timestamp().epochMicroseconds(),
        std::move(callback));
}

void
DataAccessor::getForLastWeek(GetCallback callback)
{
    const Poco::DateTime currentDateTime;
    const auto minusWeek = currentDateTime - Poco::Timespan{7, 0, 0, 0, 0};
    get(minusWeek.timestamp().epochMicroseconds(),
        currentDateTime.timestamp().epochMicroseconds(),
        std::move(callback));
}

void
DataAccessor::getForLastMonth(GetCallback callback)
{
    const Poco::DateTime currentDateTime;
    const auto minusMonth = currentDateTime - Poco::Timespan{30, 0, 0, 0, 0};
    get(minusMonth.timestamp().epochMicroseconds(),
        currentDateTime.timestamp().epochMicroseconds(),
        std::move(callback));
}

} // namespace storage
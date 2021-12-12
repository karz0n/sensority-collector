#include "storage/DataAccessor.hpp"

#include "common/Logger.hpp"
#include "common/Utils.hpp"

#include <Poco/DateTime.h>

namespace storage {

DataAccessor::DataAccessor(IDataStorage::Ptr storage)
    : _storage{std::move(storage)}
{
}

void
DataAccessor::put(const std::string& input, PutCallback callback)
{
    try {
        _storage->process(makePutJob(input, std::move(callback)));
    } catch (const std::exception& e) {
        LOG_ERROR(e.what());
        invokeIf(callback, false);
    }
}

void
DataAccessor::get(int64_t from, int64_t to, GetCallback callback)
{
    try {
        _storage->process(makeGetJob(from, to, std::move(callback)));
    } catch (const std::exception& e) {
        LOG_ERROR(e.what());
        invokeIf(callback, IDataModel::Ptr{}, false);
    }
}

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
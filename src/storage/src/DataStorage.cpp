#include "storage/DataStorage.hpp"

#include <Poco/Data/SQLite/Connector.h>

using namespace Poco::Data::Keywords;

// clang-format off
static const std::string_view TemperatureDefinition{
    "CREATE TABLE IF NOT EXISTS TemperatureData (Value REAL, Raw REAL, Timestamp INTEGER)"
};
static const std::string_view HumidityDefinition{
    "CREATE TABLE IF NOT EXISTS HumidityData (Value REAL, Raw REAL, Timestamp INTEGER)"
};
static const std::string_view PressureDefinition{
    "CREATE TABLE IF NOT EXISTS PressureData (Value REAL, Timestamp INTEGER)"
};
static const std::string_view IaqDefinition{
    "CREATE TABLE IF NOT EXISTS IaqData (Value REAL, Raw REAL, Accuracy REAL, Timestamp INTEGER)"
};
static const std::string_view GasDefinition {
    "CREATE TABLE IF NOT EXISTS GasData (Value REAL, Timestamp INTEGER)"
};
static const std::string_view Co2Definition {
    "CREATE TABLE IF NOT EXISTS Co2Data (Value REAL, Accuracy REAL, Equivalent REAL, Timestamp INTEGER)"
};
static const std::string_view EvocDefinition {
    "CREATE TABLE IF NOT EXISTS EvocData (Value REAL, Accuracy REAL, Equivalent REAL, Timestamp INTEGER)"
};
static const std::string_view TvocDefinition {
    "CREATE TABLE IF NOT EXISTS TvocData (Value REAL, Timestamp INTEGER)"
};
// clang-format on

namespace storage {

DataStorage::DataStorage(const std::string& connectionString)
    : _session{"SQLite", connectionString}
    , _terminate{false}
{
}

DataStorage::~DataStorage()
{
    terminate();
}

void
DataStorage::initialize()
{
    if (_thread.joinable()) {
        throw Poco::RuntimeException{""};
    }

    _session << TemperatureDefinition, now;
    _session << HumidityDefinition, now;
    _session << PressureDefinition, now;
    _session << IaqDefinition, now;
    _session << GasDefinition, now;
    _session << Co2Definition, now;
    _session << EvocDefinition, now;
    _session << TvocDefinition, now;

    _thread = std::thread(&DataStorage::loop, this);
}

void
DataStorage::uninitialize()
{
    terminate();
}

std::future<bool>
DataStorage::process(IDataAccessor::Ptr accessor)
{
    std::unique_lock lock{_mutex};
    _tasks.emplace(std::move(accessor));
    auto output = _tasks.back().done.get_future();
    lock.unlock();
    _cv.notify_one();
    return output;
}

void
DataStorage::setUp()
{
    Poco::Data::SQLite::Connector::registerConnector();
}

void
DataStorage::tearDown()
{
    Poco::Data::SQLite::Connector::unregisterConnector();
}

void
DataStorage::loop()
{
    while (!_terminate) {
        std::unique_lock lock{_mutex};
        _cv.wait(lock, [this]() { return _terminate || !_tasks.empty(); });
        if (_terminate) {
            break;
        }

        while (!_tasks.empty()) {
            auto& task = _tasks.front();
            try {
                task.accessor->access(_session);
                task.done.set_value(true);
            } catch (...) {
                try {
                    task.done.set_exception(std::current_exception());
                } catch (...) {
                    // Suppress exception
                }
            }
            _tasks.pop();
        }
    }
}

void
DataStorage::terminate()
{
    if (!_thread.joinable()) {
        return;
    }

    std::unique_lock lock{_mutex};
    _terminate = true;
    lock.unlock();
    _cv.notify_one();

    if (_thread.joinable()) {
        _thread.join();
    }
}

} // namespace storage
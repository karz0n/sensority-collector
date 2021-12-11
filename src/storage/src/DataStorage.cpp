#include "storage/DataStorage.hpp"

#include "common/Logger.hpp"

#include <Poco/Data/SQLite/Connector.h>

using Poco::Data::Statement;
using Poco::Data::Connector;

using namespace Poco::Data::Keywords;

// clang-format off
static const std::string_view TemperatureDataDefinition{
    "CREATE TABLE IF NOT EXISTS TemperatureData (Value REAL,Raw REAL,Timestamp INTEGER)"
};
static const std::string_view HumidityDataDefinition{
    "CREATE TABLE IF NOT EXISTS HumidityData (Value REAL,Raw REAL,Timestamp INTEGER)"
};
static const std::string_view PressureDataDefinition{
    "CREATE TABLE IF NOT EXISTS PressureData (Value REAL,Timestamp INTEGER)"
};
static const std::string_view IaqDataDefinition{
    "CREATE TABLE IF NOT EXISTS IaqData (Value REAL,Accuracy REAL,StaticValue REAL,StaticValueAccuracy REAL,Timestamp INTEGER)"
};
static const std::string_view GasDataDefinition {
    "CREATE TABLE IF NOT EXISTS GasData (Value REAL,Percentage REAL,PercentageAccuracy REAL,Timestamp INTEGER)"
};
static const std::string_view Co2DataDefinition {
    "CREATE TABLE IF NOT EXISTS Co2Data (Equivalent REAL,Accuracy REAL,Timestamp INTEGER)"
};
static const std::string_view BreathVocDataDefinition {
    "CREATE TABLE IF NOT EXISTS BreathVocData (Equivalent REAL,Accuracy REAL,Timestamp INTEGER)"
};
static const std::string_view TvocDataDefinition {
    "CREATE TABLE IF NOT EXISTS TvocData (Value REAL,Timestamp INTEGER)"
};
// clang-format on

namespace storage {

DataStorage::DataStorage(const std::string& connectionString)
    : _session{Poco::Data::SQLite::Connector::KEY, connectionString}
    , _active{false}
{
}

DataStorage::~DataStorage()
{
    terminate();
}

void
DataStorage::setUp()
{
    if (_active) {
        throw Poco::RuntimeException{"Thread is already active"};
    }

    _session << TemperatureDataDefinition, now;
    _session << HumidityDataDefinition, now;
    _session << PressureDataDefinition, now;
    _session << IaqDataDefinition, now;
    _session << GasDataDefinition, now;
    _session << Co2DataDefinition, now;
    _session << BreathVocDataDefinition, now;
    _session << TvocDataDefinition, now;

    _active = true;
    _thread = std::thread(&DataStorage::loop, this);
}

void
DataStorage::tearDown()
{
    terminate();
}

void
DataStorage::process(IDataJob::Ptr job)
{
    std::unique_lock lock{_mutex};
    _jobs.emplace(std::move(job));
    lock.unlock();
    _cv.notify_one();
}

void
DataStorage::initialize()
{
    Poco::Data::SQLite::Connector::registerConnector();
}

void
DataStorage::uninitialize()
{
    Poco::Data::SQLite::Connector::unregisterConnector();
}

void
DataStorage::loop()
{
    while (_active) {
        std::unique_lock lock{_mutex};
        _cv.wait(lock, [this]() { return !_active || !_jobs.empty(); });
        if (!_active) {
            break;
        }

        while (!_jobs.empty()) {
            auto& job = _jobs.front();
            try {
                job->run(_session);
            } catch (const std::exception& e) {
                LOG_ERROR(e.what());
            }
            _jobs.pop();
        }
    }
}

void
DataStorage::terminate()
{
    if (!_active) {
        return;
    }

    std::unique_lock lock{_mutex};
    _active = false;
    lock.unlock();
    _cv.notify_one();

    if (_thread.joinable()) {
        _thread.join();
    }
}

} // namespace storage
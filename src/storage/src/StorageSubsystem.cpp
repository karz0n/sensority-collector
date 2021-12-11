/**
 * @file StorageSubsystem.cpp
 *
 * @brief Storage subsystem definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "storage/StorageSubsystem.hpp"

#include "common/Constants.hpp"
#include "common/Logger.hpp"
#include "connectivity/ConnectivitySubsystem.hpp"
#include "storage/CompositeDataObserver.hpp"
#include "storage/SensorDataObserver.hpp"
#include "storage/DataStorage.hpp"
#include "storage/accessor/BreathVocDataAccessor.hpp"
#include "storage/accessor/Co2DataAccessor.hpp"
#include "storage/accessor/GasDataAccessor.hpp"
#include "storage/accessor/HumidityDataAccessor.hpp"
#include "storage/accessor/IaqDataAccessor.hpp"
#include "storage/accessor/PressureDataAccessor.hpp"
#include "storage/accessor/TemperatureDataAccessor.hpp"
#include "storage/accessor/TvocDataAccessor.hpp"

#include <Poco/Util/Application.h>

#include <filesystem>

namespace fs = std::filesystem;

using Poco::Util::Application;
using Poco::Util::AbstractConfiguration;

using namespace connectivity;

namespace storage {

const char*
StorageSubsystem::name() const
{
    return "StorageSubsystem";
}

void
StorageSubsystem::initialize(Application& app)
{
    DataStorage::initialize();

    auto client = app.getSubsystem<ConnectivitySubsystem>().mqttClient();
    poco_assert_dbg(client);

    createDatabase(app.config());
    poco_assert_dbg(_storage);
    _storage->setUp();

    createObserver(client, app.config());
    poco_assert_dbg(_observer);
    _observer->setUp();
    _observer->subscribe();
}

void
StorageSubsystem::uninitialize()
{
    if (_storage) {
        _storage->tearDown();
    }

    if (_observer) {
        _observer->unsubscribe();
        _observer->tearDown();
    }

    DataStorage::uninitialize();
}

void
StorageSubsystem::createDatabase(const Poco::Util::AbstractConfiguration& config)
{
    std::string connectionString{":memory:"};

    const fs::path path = config.getString(property::DatabasePath, database::Name);
    if (path.has_parent_path()) {
        const auto parent = path.parent_path();
        std::error_code ec;
        fs::create_directories(parent, ec);
        if (ec) {
            LOG_ERROR_F("Failed to create <%s> directory path", parent.string());
        } else {
            connectionString = path.string();
        }
    }

    LOG_INFO_F("Initialize database with connection string: %s", connectionString);
    _storage = std::make_shared<DataStorage>(connectionString);
}

void
StorageSubsystem::createObserver(const IMqttClient::Ptr& client,
                                 const AbstractConfiguration& config)
{
    _observer = std::make_shared<CompositeDataObserver>();

    auto accessor1 = std::make_unique<TemperatureDataAccessor>(_storage);
    const auto topic1 = config.getString(property::DataTopicsTopic1, topic::Topic1);
    _observer->add(SensorDataObserver::create(client, topic1, std::move(accessor1)));
    LOG_DEBUG_F("Register on topic1 with <%s> name", topic1);

    auto accessor2 = std::make_unique<PressureDataAccessor>(_storage);
    const auto topic2 = config.getString(property::DataTopicsTopic2, topic::Topic2);
    _observer->add(SensorDataObserver::create(client, topic2, std::move(accessor2)));
    LOG_DEBUG_F("Register on topic2 with <%s> name", topic2);

    auto accessor3 = std::make_unique<GasDataAccessor>(_storage);
    const auto topic3 = config.getString(property::DataTopicsTopic3, topic::Topic3);
    _observer->add(SensorDataObserver::create(client, topic3, std::move(accessor3)));
    LOG_DEBUG_F("Register on topic3 with <%s> name", topic3);

    auto accessor4 = std::make_unique<HumidityDataAccessor>(_storage);
    const auto topic4 = config.getString(property::DataTopicsTopic4, topic::Topic4);
    _observer->add(SensorDataObserver::create(client, topic4, std::move(accessor4)));
    LOG_DEBUG_F("Register on topic4 with <%s> name", topic4);

    auto accessor5 = std::make_unique<IaqDataAccessor>(_storage);
    const auto topic5 = config.getString(property::DataTopicsTopic5, topic::Topic5);
    _observer->add(SensorDataObserver::create(client, topic5, std::move(accessor5)));
    LOG_DEBUG_F("Register on topic5 with <%s> name", topic5);

    auto accessor6 = std::make_unique<Co2DataAccessor>(_storage);
    const auto topic6 = config.getString(property::DataTopicsTopic6, topic::Topic6);
    _observer->add(SensorDataObserver::create(client, topic6, std::move(accessor6)));
    LOG_DEBUG_F("Register on topic6 with <%s> name", topic6);

    auto accessor7 = std::make_unique<BreathVocDataAccessor>(_storage);
    const auto topic7 = config.getString(property::DataTopicsTopic7, topic::Topic7);
    _observer->add(SensorDataObserver::create(client, topic7, std::move(accessor7)));
    LOG_DEBUG_F("Register on topic7 with <%s> name", topic7);

    auto accessor8 = std::make_unique<TvocDataAccessor>(_storage);
    const auto topic8 = config.getString(property::DataTopicsTopic8, topic::Topic8);
    _observer->add(SensorDataObserver::create(client, topic8, std::move(accessor8)));
    LOG_DEBUG_F("Register on topic8 with <%s> name", topic8);
}

} // namespace storage

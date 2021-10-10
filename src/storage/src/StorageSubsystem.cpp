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
#include "storage/accessor/TemperatureData.hpp"
#include "storage/accessor/PressureData.hpp"
#include "storage/accessor/GasData.hpp"
#include "storage/accessor/HumidityData.hpp"
#include "storage/accessor/IaqData.hpp"
#include "storage/accessor/Co2Data.hpp"
#include "storage/accessor/EvocData.hpp"
#include "storage/accessor/TvocData.hpp"
#include "storage/SensorDataObserver.hpp"
#include "storage/CompositeDataObserver.hpp"

#include <Poco/Util/Application.h>

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
    _storage = std::make_shared<DataStorage>();
    _storage->setUp();

    auto client = app.getSubsystem<ConnectivitySubsystem>().mqttClient();
    poco_assert_dbg(client);
    _observer = configureObservers(client, app.config());
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
}

DataObserver::Ptr
StorageSubsystem::configureObservers(IMqttClient::Ptr client, const AbstractConfiguration& config)
{
    auto observer = std::make_shared<CompositeDataObserver>();

    auto writer1 = std::make_unique<TemperatureDataWriter>();
    const auto topic1 = config.getString(property::DataTopicsTopic1, topic::Topic1);
    observer->add(SensorDataObserver::create(client, _storage, topic1, std::move(writer1)));
    LOG_DEBUG_F("Register on topic1 with <%s> name", topic1);

    auto writer2 = std::make_unique<PressureDataWriter>();
    const auto topic2 = config.getString(property::DataTopicsTopic2, topic::Topic2);
    observer->add(SensorDataObserver::create(client, _storage, topic2, std::move(writer2)));
    LOG_DEBUG_F("Register on topic2 with <%s> name", topic2);

    auto writer3 = std::make_unique<GasDataWriter>();
    const auto topic3 = config.getString(property::DataTopicsTopic3, topic::Topic3);
    observer->add(SensorDataObserver::create(client, _storage, topic3, std::move(writer3)));
    LOG_DEBUG_F("Register on topic3 with <%s> name", topic3);

    auto writer4 = std::make_unique<HumidityDataWriter>();
    const auto topic4 = config.getString(property::DataTopicsTopic4, topic::Topic4);
    observer->add(SensorDataObserver::create(client, _storage, topic4, std::move(writer4)));
    LOG_DEBUG_F("Register on topic4 with <%s> name", topic4);

    auto writer5 = std::make_unique<IaqDataWriter>();
    const auto topic5 = config.getString(property::DataTopicsTopic5, topic::Topic5);
    observer->add(SensorDataObserver::create(client, _storage, topic5, std::move(writer5)));
    LOG_DEBUG_F("Register on topic5 with <%s> name", topic5);

    auto writer6 = std::make_unique<Co2DataWriter>();
    const auto topic6 = config.getString(property::DataTopicsTopic6, topic::Topic6);
    observer->add(SensorDataObserver::create(client, _storage, topic6, std::move(writer6)));
    LOG_DEBUG_F("Register on topic6 with <%s> name", topic6);

    auto writer7 = std::make_unique<EvocDataWriter>();
    const auto topic7 = config.getString(property::DataTopicsTopic7, topic::Topic7);
    observer->add(SensorDataObserver::create(client, _storage, topic7, std::move(writer7)));
    LOG_DEBUG_F("Register on topic7 with <%s> name", topic7);

    auto writer8 = std::make_unique<TvocDataWriter>();
    const auto topic8 = config.getString(property::DataTopicsTopic8, topic::Topic8);
    observer->add(SensorDataObserver::create(client, _storage, topic8, std::move(writer8)));
    LOG_DEBUG_F("Register on topic8 with <%s> name", topic8);

    return observer;
}

} // namespace storage

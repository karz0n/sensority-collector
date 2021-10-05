/**
 * @file StorageSubsystem.cpp
 *
 * @brief Storage subsystem definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "storage/StorageSubsystem.hpp"

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
    auto client = app.getSubsystem<ConnectivitySubsystem>().mqttClient();

    _storage = std::make_shared<DataStorage>();
    _observer = std::make_shared<CompositeDataObserver>();

    auto writer1 = std::make_unique<TemperatureDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "temperature", std::move(writer1)));
    auto writer2 = std::make_unique<PressureDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "pressure", std::move(writer2)));
    auto writer3 = std::make_unique<GasDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "gas", std::move(writer3)));
    auto writer4 = std::make_unique<HumidityDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "humidity", std::move(writer4)));
    auto writer5 = std::make_unique<IaqDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "iaq", std::move(writer5)));
    auto writer6 = std::make_unique<Co2DataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "eCO2", std::move(writer6)));
    auto writer7 = std::make_unique<EvocDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "eVOC", std::move(writer7)));
    auto writer8 = std::make_unique<TvocDataWriter>();
    _observer->add(SensorDataObserver::create(client, _storage, "TVOC", std::move(writer8)));

    _storage->initialize();

    _observer->initialize();
    _observer->subscribe();
}

void
StorageSubsystem::uninitialize()
{
    if (_storage) {
        _storage->uninitialize();
    }

    if (_observer) {
        _observer->unsubscribe();
        _observer->uninitialize();
    }
}

} // namespace storage

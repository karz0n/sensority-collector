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
#include "common/Logger.hpp"

#include "storage/DataWriter.hpp"

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

    auto tempDataWriter = std::make_unique<TemperatureDataWriter>();
    _tempDataObserver = DataObserver::create(client, "temperature", std::move(tempDataWriter));
    _tempDataObserver->initialize();
    _tempDataObserver->subscribe();

    auto pressureDataWriter = std::make_unique<PressureDataWriter>();
    _pressureDataObserver = DataObserver::create(client, "pressure", std::move(pressureDataWriter));
    _pressureDataObserver->initialize();
    _pressureDataObserver->subscribe();

    auto gasDataWriter = std::make_unique<GasDataWriter>();
    _gasDataObserver = DataObserver::create(client, "gas", std::move(gasDataWriter));
    _gasDataObserver->initialize();
    _gasDataObserver->subscribe();

    auto humidityDataWriter = std::make_unique<HumidityDataWriter>();
    _humidityDataObserver = DataObserver::create(client, "humidity", std::move(humidityDataWriter));
    _humidityDataObserver->initialize();
    _humidityDataObserver->subscribe();

    auto iaqDataWriter = std::make_unique<IaqDataWriter>();
    _iaqDataObserver = DataObserver::create(client, "iaq", std::move(iaqDataWriter));
    _iaqDataObserver->initialize();
    _iaqDataObserver->subscribe();

    auto co2DataWriter = std::make_unique<Co2DataWriter>();
    _co2DataObserver = DataObserver::create(client, "eCO2", std::move(co2DataWriter));
    _co2DataObserver->initialize();
    _co2DataObserver->subscribe();

    auto evocDataWriter = std::make_unique<EvocDataWriter>();
    _evocDataObserver = DataObserver::create(client, "eVOC", std::move(evocDataWriter));
    _evocDataObserver->initialize();
    _evocDataObserver->subscribe();

    auto tvocDataWriter = std::make_unique<TvocDataWriter>();
    _tvocDataObserver = DataObserver::create(client, "TVOC", std::move(tvocDataWriter));
    _tvocDataObserver->initialize();
    _tvocDataObserver->subscribe();
}

void
StorageSubsystem::uninitialize()
{
    _tempDataObserver->unsubscribe();
    _tempDataObserver->uninitialize();

    _pressureDataObserver->unsubscribe();
    _pressureDataObserver->uninitialize();

    _gasDataObserver->unsubscribe();
    _gasDataObserver->uninitialize();

    _humidityDataObserver->unsubscribe();
    _humidityDataObserver->uninitialize();

    _iaqDataObserver->unsubscribe();
    _iaqDataObserver->uninitialize();

    _co2DataObserver->unsubscribe();
    _co2DataObserver->uninitialize();

    _evocDataObserver->unsubscribe();
    _evocDataObserver->uninitialize();

    _tvocDataObserver->unsubscribe();
    _tvocDataObserver->uninitialize();
}

} // namespace storage

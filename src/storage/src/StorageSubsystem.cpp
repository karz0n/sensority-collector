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
#include "storage/DataWriter.hpp"
#include "storage/SensorDataObserver.hpp"

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

    auto writer1 = std::make_unique<TemperatureDataWriter>();
    _observer.add(SensorDataObserver::create(client, "temperature", std::move(writer1)));
    auto writer2 = std::make_unique<PressureDataWriter>();
    _observer.add(SensorDataObserver::create(client, "pressure", std::move(writer2)));
    auto writer3 = std::make_unique<GasDataWriter>();
    _observer.add(SensorDataObserver::create(client, "gas", std::move(writer3)));
    auto writer4 = std::make_unique<HumidityDataWriter>();
    _observer.add(SensorDataObserver::create(client, "humidity", std::move(writer4)));
    auto writer5 = std::make_unique<IaqDataWriter>();
    _observer.add(SensorDataObserver::create(client, "iaq", std::move(writer5)));
    auto writer6 = std::make_unique<Co2DataWriter>();
    _observer.add(SensorDataObserver::create(client, "eCO2", std::move(writer6)));
    auto writer7 = std::make_unique<EvocDataWriter>();
    _observer.add(SensorDataObserver::create(client, "eVOC", std::move(writer7)));
    auto writer8 = std::make_unique<TvocDataWriter>();
    _observer.add(SensorDataObserver::create(client, "TVOC", std::move(writer8)));

    _observer.initialize();
    _observer.subscribe();
}

void
StorageSubsystem::uninitialize()
{
    _observer.unsubscribe();
    _observer.uninitialize();
}

} // namespace storage

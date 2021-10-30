/**
 * @file StorageSubsystem.hpp
 *
 * @brief Storage subsystem declaration
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#pragma once

#include <Poco/Util/Subsystem.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "storage/DataObserver.hpp"
#include "storage/DataStorage.hpp"
#include "connectivity/IMqttClient.hpp"

namespace storage {

class StorageSubsystem final : public Poco::Util::Subsystem {
public:
    const char*
    name() const override;

private: /* Poco::Util::Subsystem interface */
    void
    initialize(Poco::Util::Application& app) override;

    void
    uninitialize() override;

private:
    DataObserver::Ptr
    configureObservers(const connectivity::IMqttClient::Ptr& client,
                       const Poco::Util::AbstractConfiguration& config);

private:
    DataStorage::Ptr _storage;
    DataObserver::Ptr _observer;
};

} // namespace storage

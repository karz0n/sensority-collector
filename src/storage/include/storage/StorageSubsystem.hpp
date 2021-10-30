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

#include "storage/IDataStorage.hpp"
#include "storage/DataObserver.hpp"
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
    void
    createDatabase(const Poco::Util::AbstractConfiguration& config);

    void
    createObserver(const connectivity::IMqttClient::Ptr& client,
                   const Poco::Util::AbstractConfiguration& config);

private:
    IDataStorage::Ptr _storage;
    DataObserver::Ptr _observer;
};

} // namespace storage

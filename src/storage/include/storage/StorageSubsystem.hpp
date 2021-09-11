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

#include "storage/DataObserver.hpp"

namespace storage {

class StorageSubsystem final : public Poco::Util::Subsystem {
public:
    const char*
    name() const override;

private:
    void
    initialize(Poco::Util::Application& app) override;

    void
    uninitialize() override;

private:
    DataObserver::Ptr _tempDataObserver;
    DataObserver::Ptr _humidityDataObserver;
    DataObserver::Ptr _pressureDataObserver;
    DataObserver::Ptr _gasDataObserver;
    DataObserver::Ptr _iaqDataObserver;
    DataObserver::Ptr _co2DataObserver;
    DataObserver::Ptr _evocDataObserver;
    DataObserver::Ptr _tvocDataObserver;
};

} // namespace storage

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

#include "storage/CompositeDataObserver.hpp"

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
    CompositeDataObserver _observer;
};

} // namespace storage

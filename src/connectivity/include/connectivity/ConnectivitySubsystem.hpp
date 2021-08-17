/**
 * @file ConnectivitySubsystem.hpp
 *
 * @brief Connectivity subsystem declaration
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#pragma once

#include <Poco/Util/Subsystem.h>

namespace connectivity {

class ConnectivitySubsystem final : public Poco::Util::Subsystem {
public:
    const char*
    name() const override;

private:
    void
    initialize(Poco::Util::Application&) override;

    void
    uninitialize() override;
};

} // namespace connectivity
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

#include "connectivity/IMqttClient.hpp"

namespace connectivity {

class ConnectivitySubsystem final : public Poco::Util::Subsystem {
public:
    ConnectivitySubsystem();

    const char*
    name() const override;

    IMqttClient::Ptr
    mqttClient();

private:
    void
    initialize(Poco::Util::Application&) override;

    void
    uninitialize() override;

private:
    IMqttClient::Ptr _mqttClient;
};

} // namespace connectivity
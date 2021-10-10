/**
 * @file ConnectivitySubsystem.cpp
 *
 * @brief Connectivity subsystem definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "connectivity/ConnectivitySubsystem.hpp"

#include "common/Constants.hpp"
#include "common/Logger.hpp"
#include "connectivity/MqttClient.hpp"

#include <string>

using Poco::Util::Application;

namespace connectivity {

ConnectivitySubsystem::ConnectivitySubsystem()
    : _mqttClient{std::make_shared<MqttClient>(false)}
{
}

const char*
ConnectivitySubsystem::name() const
{
    return "ConnectivitySubsystem";
}

IMqttClient::Ptr
ConnectivitySubsystem::mqttClient()
{
    return _mqttClient;
}

void
ConnectivitySubsystem::initialize(Application&)
{
    connectServer();
}

void
ConnectivitySubsystem::uninitialize()
{
    _mqttClient->disconnect();
}

void
ConnectivitySubsystem::connectServer()
{
    const auto& config = Application::instance().config();

    try {
        const bool secure = config.getBool(property::ServerSecure, true);

        if (secure) {
            if (config.has(property::ServerUsername) && config.has(property::ServerPassword)) {
                const auto username = config.getString(property::ServerUsername);
                const auto password = config.getString(property::ServerPassword);
                LOG_DEBUG("Apply server credentials");
                _mqttClient->setCredentials(username, password);
            } else {
                LOG_ERROR("Expected server credentials are missing");
            }
        }

        if (config.has(property::ServerHost) && config.has(property::ServerPort)) {
            const auto host = config.getString(property::ServerHost);
            const auto port = config.getUInt(property::ServerPort);
            LOG_DEBUG_F("Attempt connect to <%s:%d> server", host, port);
            _mqttClient->connect(host, port, MqttDefaultKeepAlive);
        } else {
            LOG_ERROR("Unable connect due to missing necessary parameters");
        }
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

} // namespace connectivity
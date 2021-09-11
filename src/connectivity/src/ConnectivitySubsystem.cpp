/**
 * @file ConnectivitySubsystem.cpp
 *
 * @brief Connectivity subsystem definition
 *
 * @author Denys Asauliak
 * Contact: d.asauliak@gmail.com
 */

#include "connectivity/ConnectivitySubsystem.hpp"

#include "common/Logger.hpp"
#include "connectivity/MqttClient.hpp"

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

IMqttClient::Ptr ConnectivitySubsystem::mqttClient()
{
    return _mqttClient;
}

void
ConnectivitySubsystem::initialize(Application&)
{
    _mqttClient->setCredentials("test", "test");
    _mqttClient->connect("192.168.1.10", 1883, MqttDefaultKeepAlive);
}

void
ConnectivitySubsystem::uninitialize()
{
    _mqttClient->disconnect();
}

} // namespace connectivity
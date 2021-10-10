#include "connectivity/MqttInitializer.hpp"

#include "common/Logger.hpp"

#include <mosquitto.h>

namespace connectivity {

MqttInitializer::MqttInitializer()
    : _initialized{false}
{
}

MqttInitializer::~MqttInitializer()
{
    uninitialize();
}

bool
MqttInitializer::initialize() noexcept
{
    poco_assert_dbg(!_initialized);
    if (_initialized) {
        LOG_ERROR("Mqtt has already initialized");
        return true;
    }

    if (mosquitto_lib_init() == MOSQ_ERR_SUCCESS) {
        _initialized = true;
        int maj{0}, min{0}, rev{0};
        mosquitto_lib_version(&maj, &min, &rev);
        LOG_INFO_F("Mosquitto library with %d.%d.%d version is initialized", maj, min, rev);
        return true;
    }

    return false;
}

void
MqttInitializer::uninitialize() noexcept
{
    if (_initialized) {
        mosquitto_lib_cleanup();
        _initialized = false;
    }
}

} // namespace connectivity

#pragma once

namespace connectivity {

class MqttInitializer final {
public:
    MqttInitializer();

    ~MqttInitializer();

    bool initialize() noexcept;

    void uninitialize() noexcept;

private:
    bool _initialized;
};

} // namespace connectivity
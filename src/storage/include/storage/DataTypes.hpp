#pragma once

#include <cstdint>

namespace storage {

struct TemperatureData {
    float value{};
    float raw{};
    int64_t timestamp{};
};

struct Co2Data {
    float equivalent{};
    float accuracy{};
    int64_t timestamp{};
};

struct BreathVocData {
    float equivalent{};
    float accuracy{};
    int64_t timestamp{};
};

struct GasData {
    float value{};
    float percentage{};
    float percentageAccuracy{};
    int64_t timestamp{};
};

struct HumidityData {
    float value{};
    float raw{};
    int64_t timestamp{};
};

struct IaqData {
    float value{};
    float accuracy{};
    float staticValue{};
    float staticValueAccuracy{};
    int64_t timestamp{};
};

struct PressureData {
    float value{};
    int64_t timestamp{};
};

struct TvocData {
    float value{};
    int64_t timestamp{};
};

} // namespace storage
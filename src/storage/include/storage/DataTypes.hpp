#pragma once

#include <cstdint>

namespace storage {

struct TempData {
    float value{};
    float raw{};
    int64_t timestamp{};
};

struct eCo2Data {
    float equivalent{};
    float accuracy{};
    int64_t timestamp{};
};

struct eBreathVocData {
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

}
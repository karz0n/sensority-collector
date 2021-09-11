#include "storage/DataWriter.hpp"

#include "common/Logger.hpp"

using Poco::JSON::Object;

namespace storage {

void
TemperatureDataWriter::process(Poco::JSON::Object::Ptr object)
{
    poco_assert_dbg(object);
    try {
        if (object) {
            const auto value = object->getValue<float>("value");
            const auto raw = object->getValue<float>("raw");
            LOG_INFO_F("Temperature: value<%hf>, raw<%hf>", value, raw);
        }
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

void
HumidityDataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        const auto raw = object->getValue<float>("raw");
        LOG_INFO_F("Humidity: value<%hf>, raw<%hf>", value, raw);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse temperature data: %s", e.message());
    }
}

void
PressureDataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        LOG_INFO_F("Pressure: value<%hf>", value);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse pressure data: %s", e.message());
    }
}

void
GasDataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        LOG_INFO_F("Gas: value<%hf>", value);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse gas data: %s", e.message());
    }
}

void
IaqDataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        const auto raw = object->getValue<float>("raw");
        const auto accuracy = object->getValue<float>("accuracy");
        LOG_INFO_F("IAQ: value<%hf>, raw<%hf>, accuracy<%hf>", value, raw, accuracy);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse IAQ data: %s", e.message());
    }
}

void
Co2DataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        const auto accuracy = object->getValue<float>("accuracy");
        const auto equivalent = object->getValue<float>("equivalent");
        LOG_INFO_F("CO2: value<%hf>, accuracy<%hf>, equivalent<%hf>", value, accuracy, equivalent);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse CO2 data: %s", e.message());
    }
}

void
EvocDataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        const auto accuracy = object->getValue<float>("accuracy");
        const auto equivalent = object->getValue<float>("equivalent");
        LOG_INFO_F("eVOC: value<%hf>, accuracy<%hf>, equivalent<%hf>", value, accuracy, equivalent);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse eVOC data: %s", e.message());
    }
}

void
TvocDataWriter::process(Poco::JSON::Object::Ptr object)
{
    try {
        const auto value = object->getValue<float>("value");
        LOG_INFO_F("TVOC: value<%hf>", value);
    } catch (const Poco::Exception& e) {
        LOG_ERROR_F("Failed to parse TVOC data: %s", e.message());
    }
}

} // namespace storage
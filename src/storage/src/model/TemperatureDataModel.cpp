#include "storage/model/TemperatureDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

namespace storage {

bool
TemperatureDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
TemperatureDataModel::stringify() const
{
    return stringifyArray();
}

bool
TemperatureDataModel::parseItem(Poco::JSON::Object::Ptr object, TemperatureData& data)
{
    poco_assert_dbg(object);

    try {
        data.value = object->getValue<float>("value");
        data.raw = object->getValue<float>("raw");
        data.timestamp = Poco::Timestamp{}.epochMicroseconds();
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
        return false;
    }

    return true;
}

Poco::JSON::Object::Ptr
TemperatureDataModel::stringifyItem(const TemperatureData& value)
{
    return {};
}

} // namespace storage
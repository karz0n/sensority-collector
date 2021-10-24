#include "storage/model/HumidityDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

namespace storage {

bool
HumidityDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
HumidityDataModel::stringify() const
{
    return stringifyArray();
}

bool
HumidityDataModel::parseItem(Poco::JSON::Object::Ptr object, HumidityData& data)
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
HumidityDataModel::stringifyItem(const HumidityData& value)
{
    return {};
}

}
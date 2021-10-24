#include "storage/model/PressureDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

namespace storage {

bool
PressureDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
PressureDataModel::stringify() const
{
    return stringifyArray();
}

bool
PressureDataModel::parseItem(Poco::JSON::Object::Ptr object, PressureData& data)
{
    poco_assert_dbg(object);

    try {
        data.value = object->getValue<float>("value");
        data.timestamp = Poco::Timestamp{}.epochMicroseconds();
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
        return false;
    }

    return true;
}

Poco::JSON::Object::Ptr
PressureDataModel::stringifyItem(const PressureData& value)
{
    return {};
}

}
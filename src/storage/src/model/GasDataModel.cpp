#include "storage/model/GasDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

using Poco::Timestamp;

namespace storage {

bool
GasDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
GasDataModel::stringify() const
{
    return stringifyArray();
}

bool
GasDataModel::parseItem(Poco::JSON::Object::Ptr object, GasData& data)
{
    poco_assert_dbg(object);

    try {
        data.value = object->getValue<float>("value");
        data.percentage = object->getValue<float>("percentage");
        data.percentageAccuracy = object->getValue<float>("percentageAccuracy");
        data.timestamp = Timestamp{}.epochMicroseconds();
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
        return false;
    }

    return true;
}

Poco::JSON::Object::Ptr
GasDataModel::stringifyItem(const GasData& value)
{
    return {};
}

} // namespace storage
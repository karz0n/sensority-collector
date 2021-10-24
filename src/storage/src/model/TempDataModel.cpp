#include "storage/model/TempDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

namespace storage {

bool
TempDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
TempDataModel::stringify() const
{
    return stringifyArray();
}

bool
TempDataModel::parseItem(Poco::JSON::Object::Ptr object, TempData& data)
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
TempDataModel::stringifyItem(const TempData& value)
{
    return {};
}

} // namespace storage
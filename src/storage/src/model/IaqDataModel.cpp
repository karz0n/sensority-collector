#include "storage/model/IaqDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

namespace storage {

bool
IaqDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
IaqDataModel::stringify() const
{
    return stringifyArray();
}

bool
IaqDataModel::parseItem(Poco::JSON::Object::Ptr object, IaqData& data)
{
    poco_assert_dbg(object);

    try {
        data.value = object->getValue<float>("value");
        data.accuracy = object->getValue<float>("accuracy");
        data.staticValue = object->getValue<float>("staticValue");
        data.staticValueAccuracy = object->getValue<float>("staticValueAccuracy");
        data.timestamp = Poco::Timestamp{}.epochMicroseconds();
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
        return false;
    }

    return true;
}

Poco::JSON::Object::Ptr
IaqDataModel::stringifyItem(const IaqData& value)
{
    return {};
}

} // namespace storage
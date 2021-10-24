#include "storage/model/TvocDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

namespace storage {

bool
TvocDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
TvocDataModel::stringify() const
{
    return stringifyArray();
}

bool
TvocDataModel::parseItem(Poco::JSON::Object::Ptr object, TvocData& data)
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
TvocDataModel::stringifyItem(const TvocData& value)
{
    return {};
}

} // namespace storage
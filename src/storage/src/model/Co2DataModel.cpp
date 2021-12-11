#include "storage/model/Co2DataModel.hpp"

#include "common/Logger.hpp"

using Poco::Timestamp;

namespace storage {

bool
Co2DataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
Co2DataModel::stringify() const
{
    return stringifyArray();
}

bool
Co2DataModel::parseItem(Poco::JSON::Object::Ptr object, Co2Data& data)
{
    poco_assert_dbg(object);

    try {
        data.equivalent = object->getValue<float>("equivalent");
        data.accuracy = object->getValue<float>("accuracy");
        data.timestamp = Timestamp{}.epochMicroseconds();
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
        return false;
    }

    return true;
}

Poco::JSON::Object::Ptr
Co2DataModel::stringifyItem(const Co2Data& value)
{
    return {};
}

} // namespace storage
#include "storage/model/eCo2DataModel.hpp"

#include "common/Logger.hpp"

using Poco::Timestamp;

namespace storage {

bool
eCo2DataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
eCo2DataModel::stringify() const
{
    return stringifyArray();
}

bool
eCo2DataModel::parseItem(Poco::JSON::Object::Ptr object, eCo2Data& data)
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
eCo2DataModel::stringifyItem(const eCo2Data& value)
{
    return {};
}

} // namespace storage
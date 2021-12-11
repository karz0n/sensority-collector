#include "storage/model/BreathVocDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

using Poco::Timestamp;

namespace storage {

bool
BreathVocDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
BreathVocDataModel::stringify() const
{
    return stringifyArray();
}

bool
BreathVocDataModel::parseItem(Poco::JSON::Object::Ptr object, BreathVocData& data)
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
BreathVocDataModel::stringifyItem(const BreathVocData& value)
{
    return {};
}

} // namespace storage
#include "storage/model/eBreathVocDataModel.hpp"

#include "common/Logger.hpp"

#include <Poco/Timestamp.h>

using Poco::Timestamp;

namespace storage {

bool
eBreathVocDataModel::parse(const std::string& input)
{
    return parseArray(input);
}

std::string
eBreathVocDataModel::stringify() const
{
    return stringifyArray();
}

bool
eBreathVocDataModel::parseItem(Poco::JSON::Object::Ptr object, eBreathVocData& data)
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
eBreathVocDataModel::stringifyItem(const eBreathVocData& value)
{
    return {};
}

} // namespace storage
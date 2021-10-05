#include "storage/accessor/HumidityData.hpp"

#include "common/Logger.hpp"

#include <Poco/JSON/Parser.h>
#include <Poco/Timestamp.h>

using Poco::JSON::Object;
using Poco::JSON::Parser;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Timestamp;

using namespace Poco::Data::Keywords;

namespace storage {

HumidityDataReader::HumidityDataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
HumidityDataReader::stringify()
{
    return {};
}

void
HumidityDataReader::access(Session& session)
{
    try {
        HumidityData data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Raw, Timestamp FROM HumidityData",
            into(data.value),
            into(data.raw),
            into(data.timestamp),
            range(_from, _to);
        // clang-format on

        _records.clear();
        _records.reserve(_to - _from);
        while (!select.done()) {
            select.execute();
            _records.push_back(data);
        }
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

HumidityDataReader::Ptr
HumidityDataReader::clone()
{
    return std::make_unique<HumidityDataReader>(_from, _to);
}

HumidityDataReader::Records&
HumidityDataReader::records()
{
    return _records;
}

const HumidityDataReader::Records&
HumidityDataReader::records() const
{
    return _records;
}

bool
HumidityDataWriter::parse(const std::string& input)
{
    try {
        Parser parser;
        if (auto var = parser.parse(input); !var.isEmpty()) {
            if (auto object = var.extract<Object::Ptr>(); object) {
                _data.value = object->getValue<float>("value");
                _data.raw = object->getValue<float>("raw");
                _data.timestamp = Timestamp{}.epochMicroseconds();
                return true;
            }
        }
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
    return false;
}

void
HumidityDataWriter::access(Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO HumidityData VALUES (?, ?, ?)",
            use(_data.value),
            use(_data.raw),
            use(_data.timestamp),
            now;
        // clang-format on
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

IDataWriter::Ptr
HumidityDataWriter::clone()
{
    return std::make_unique<HumidityDataWriter>();
}

} // namespace storage
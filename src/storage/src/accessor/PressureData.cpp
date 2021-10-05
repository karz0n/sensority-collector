#include "storage/accessor/PressureData.hpp"

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

PressureDataReader::PressureDataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
PressureDataReader::stringify()
{
    return {};
}

void
PressureDataReader::access(Session& session)
{
    try {
        PressureData data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Timestamp FROM PressureData",
            into(data.value),
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

PressureDataReader::Ptr
PressureDataReader::clone()
{
    return std::make_unique<PressureDataReader>(_from, _to);
}

PressureDataReader::Records&
PressureDataReader::records()
{
    return _records;
}

const PressureDataReader::Records&
PressureDataReader::records() const
{
    return _records;
}

bool
PressureDataWriter::parse(const std::string& input)
{
    try {
        Parser parser;
        if (auto var = parser.parse(input); !var.isEmpty()) {
            if (auto object = var.extract<Object::Ptr>(); object) {
                _data.value = object->getValue<float>("value");
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
PressureDataWriter::access(Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO PressureData VALUES (?, ?)",
            use(_data.value),
            use(_data.timestamp),
            now;
        // clang-format on
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

IDataWriter::Ptr
PressureDataWriter::clone()
{
    return std::make_unique<PressureDataWriter>();
}

} // namespace storage
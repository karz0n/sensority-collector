#include "storage/accessor/GasData.hpp"

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

GasDataReader::GasDataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
GasDataReader::stringify()
{
    return {};
}

void
GasDataReader::access(Poco::Data::Session& session)
{
    try {
        GasData data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Timestamp FROM GasData",
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

GasDataReader::Ptr
GasDataReader::clone()
{
    return std::make_unique<GasDataReader>(_from, _to);
}

GasDataReader::Records&
GasDataReader::records()
{
    return _records;
}

const GasDataReader::Records&
GasDataReader::records() const
{
    return _records;
}

bool
GasDataWriter::parse(const std::string& input)
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
GasDataWriter::access(Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO GasData VALUES (?, ?)",
            use(_data.value),
            use(_data.timestamp),
            now;
        // clang-format on
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

IDataWriter::Ptr
GasDataWriter::clone()
{
    return std::make_unique<GasDataWriter>();
}

} // namespace storage
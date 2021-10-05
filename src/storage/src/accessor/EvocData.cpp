#include "storage/accessor/EvocData.hpp"

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

EvocDataReader::EvocDataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
EvocDataReader::stringify()
{
    return {};
}

void
EvocDataReader::access(Poco::Data::Session& session)
{
    try {
        EvocData data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Accuracy, Equivalent, Timestamp FROM EvocData",
            into(data.value),
            into(data.accuracy),
            into(data.equivalent),
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

EvocDataReader::Ptr
EvocDataReader::clone()
{
    return std::make_unique<EvocDataReader>(_from, _to);
}

EvocDataReader::Records&
EvocDataReader::records()
{
    return _records;
}

const EvocDataReader::Records&
EvocDataReader::records() const
{
    return _records;
}

bool
EvocDataWriter::parse(const std::string& input)
{
    try {
        Parser parser;
        if (auto var = parser.parse(input); !var.isEmpty()) {
            if (auto object = var.extract<Object::Ptr>(); object) {
                _data.value = object->getValue<float>("value");
                _data.accuracy = object->getValue<float>("accuracy");
                _data.equivalent = object->getValue<float>("equivalent");
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
EvocDataWriter::access(Poco::Data::Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO EvocData VALUES (?, ?, ?, ?)",
            use(_data.value),
            use(_data.accuracy),
            use(_data.equivalent),
            use(_data.timestamp),
            now;
        // clang-format on
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

IDataWriter::Ptr
EvocDataWriter::clone()
{
    return std::make_unique<EvocDataWriter>();
}

} // namespace storage
#include "storage/accessor/TvocData.hpp"

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

TvocDataReader::TvocDataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
TvocDataReader::stringify()
{
    return {};
}

void
TvocDataReader::access(Poco::Data::Session& session)
{
    try {
        TvocData data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Timestamp FROM TvocData",
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

TvocDataReader::Ptr
TvocDataReader::clone()
{
    return std::make_unique<TvocDataReader>(_from, _to);
}

TvocDataReader::Records&
TvocDataReader::records()
{
    return _records;
}

const TvocDataReader::Records&
TvocDataReader::records() const
{
    return _records;
}

bool
TvocDataWriter::parse(const std::string& input)
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
TvocDataWriter::access(Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO TvocData VALUES (?, ?)",
            use(_data.value),
            use(_data.timestamp),
            now;
        // clang-format on
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

IDataWriter::Ptr
TvocDataWriter::clone()
{
    return std::make_unique<TvocDataWriter>();
}

} // namespace storage

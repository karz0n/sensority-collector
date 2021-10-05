#include "storage/accessor/IaqData.hpp"

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

IaqDataReader::IaqDataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
IaqDataReader::stringify()
{
    return {};
}

void
IaqDataReader::access(Session& session)
{
    try {
        IaqData data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Raw, Accuracy, Timestamp FROM IaqData",
            into(data.value),
            into(data.raw),
            into(data.accuracy),
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

IaqDataReader::Ptr
IaqDataReader::clone()
{
    return std::make_unique<IaqDataReader>(_from, _to);
}

IaqDataReader::Records&
IaqDataReader::records()
{
    return _records;
}

const IaqDataReader::Records&
IaqDataReader::records() const
{
    return _records;
}

bool
IaqDataWriter::parse(const std::string& input)
{
    try {
        Parser parser;
        if (auto var = parser.parse(input); !var.isEmpty()) {
            if (auto object = var.extract<Object::Ptr>(); object) {
                _data.value = object->getValue<float>("value");
                _data.raw = object->getValue<float>("raw");
                _data.accuracy = object->getValue<float>("accuracy");
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
IaqDataWriter::access(Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO IaqData VALUES (?, ?, ?, ?)",
            use(_data.value),
            use(_data.raw),
            use(_data.accuracy),
            use(_data.timestamp),
            now;
        // clang-format on
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
    }
}

IDataWriter::Ptr
IaqDataWriter::clone()
{
    return std::make_unique<IaqDataWriter>();
}

} // namespace storage
#include "storage/accessor/Co2Data.hpp"

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

Co2DataReader::Co2DataReader(std::size_t from, std::size_t to)
    : _from{from}
    , _to{to}
{
    poco_assert_dbg(_from >= 0 && _to > _from);
}

std::string
Co2DataReader::stringify()
{
    return {};
}

void
Co2DataReader::access(Session& session)
{
    try {
        Co2Data data;

        // clang-format off
        Statement select{session};
        select << "SELECT Value, Accuracy, Equivalent, Timestamp FROM Co2Data",
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

Co2DataReader::Ptr
Co2DataReader::clone()
{
    return std::make_unique<Co2DataReader>(_from, _to);
}

Co2DataReader::Records&
Co2DataReader::records()
{
    return _records;
}

const Co2DataReader::Records&
Co2DataReader::records() const
{
    return _records;
}

bool
Co2DataWriter::parse(const std::string& input)
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
Co2DataWriter::access(Session& session)
{
    try {
        // clang-format off
        session << "INSERT INTO Co2Data VALUES (?, ?, ?, ?)",
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
Co2DataWriter::clone()
{
    return std::make_unique<Co2DataWriter>();
}

} // namespace storage
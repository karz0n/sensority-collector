#pragma once

#include "storage/IDataWriter.hpp"
#include "storage/IDataReader.hpp"

namespace storage {

struct PressureData {
    float value;
    int64_t timestamp;
};

class PressureDataReader final : public IDataReader {
public:
    using Records = std::vector<PressureData>;

    PressureDataReader(std::size_t from = 0, std::size_t to = 1);

    std::string
    stringify() override;

    void
    access(Poco::Data::Session& session) override;

    Ptr
    clone() override;

    Records&
    records();

    const Records&
    records() const;

private:
    std::size_t _from;
    std::size_t _to;
    Records _records;
};

class PressureDataWriter : public IDataWriter {
public:
    bool
    parse(const std::string& input) override;

    void
    access(Poco::Data::Session& session) override;

    IDataWriter::Ptr
    clone() override;

private:
    PressureData _data;
};

} // namespace storage
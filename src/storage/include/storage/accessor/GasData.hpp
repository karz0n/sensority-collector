#pragma once

#include "storage/IDataWriter.hpp"
#include "storage/IDataReader.hpp"

namespace storage {

struct GasData {
    float value;
    int64_t timestamp;
};

class GasDataReader final : public IDataReader {
public:
    using Records = std::vector<GasData>;

    GasDataReader(std::size_t from = 0, std::size_t to = 1);

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

class GasDataWriter final : public IDataWriter {
public:
    bool
    parse(const std::string& input) override;

    void
    access(Poco::Data::Session& session) override;

    IDataWriter::Ptr
    clone() override;

private:
    GasData _data;
};

} // namespace storage
#pragma once

#include <string>
#include <memory>

#include <Poco/JSON/Object.h>

namespace storage {

class IDataWriter {
public:
    using Ptr = std::shared_ptr<IDataWriter>;

    virtual ~IDataWriter() = default;

    virtual void
    process(Poco::JSON::Object::Ptr object)
        = 0;
};

class TemperatureDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class HumidityDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class PressureDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class GasDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class IaqDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class Co2DataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class EvocDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

class TvocDataWriter : public IDataWriter {
public:
    void
    process(Poco::JSON::Object::Ptr object) override;
};

} // namespace storage
#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

#include <memory>

namespace storage {

class TemperatureDataModel : public TypedDataModel<TemperatureData> {
public:
    using Ptr = std::shared_ptr<TemperatureDataModel>;

    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, TemperatureData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const TemperatureData& value) override;
};

} // namespace storage
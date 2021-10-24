#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class HumidityDataModel : public TypedDataModel<HumidityData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, HumidityData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const HumidityData& value) override;
};

}
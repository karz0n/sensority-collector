#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class PressureDataModel : public TypedDataModel<PressureData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, PressureData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const PressureData& value) override;
};

}
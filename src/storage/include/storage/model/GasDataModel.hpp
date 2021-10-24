#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class GasDataModel : public TypedDataModel<GasData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, GasData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const GasData& value) override;
};

}
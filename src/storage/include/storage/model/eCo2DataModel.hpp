#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class eCo2DataModel : public TypedDataModel<eCo2Data> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, eCo2Data& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const eCo2Data& value) override;
};

}
#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class Co2DataModel : public TypedDataModel<Co2Data> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, Co2Data& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const Co2Data& value) override;
};

} // namespace storage
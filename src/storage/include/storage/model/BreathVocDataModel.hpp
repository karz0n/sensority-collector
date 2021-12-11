#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class BreathVocDataModel : public TypedDataModel<BreathVocData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, BreathVocData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const BreathVocData& value) override;
};

} // namespace storage
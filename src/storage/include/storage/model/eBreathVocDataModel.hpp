#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class eBreathVocDataModel : public TypedDataModel<eBreathVocData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, eBreathVocData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const eBreathVocData& value) override;
};

}
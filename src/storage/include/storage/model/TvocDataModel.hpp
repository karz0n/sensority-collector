#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class TvocDataModel : public TypedDataModel<TvocData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, TvocData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const TvocData& value) override;
};

} // namespace storage
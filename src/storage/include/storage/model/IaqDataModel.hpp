#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

namespace storage {

class IaqDataModel : public TypedDataModel<IaqData> {
public:
    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, IaqData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const IaqData& value) override;
};

}
#pragma once

#include "storage/TypedDataModel.hpp"
#include "storage/DataTypes.hpp"

#include <memory>

namespace storage {

class TempDataModel : public TypedDataModel<TempData> {
public:
    using Ptr = std::shared_ptr<TempDataModel>;

    [[nodiscard]] bool
    parse(const std::string& input) override;

    [[nodiscard]] std::string
    stringify() const override;

private:
    bool
    parseItem(Poco::JSON::Object::Ptr object, TempData& data) override;

    Poco::JSON::Object::Ptr
    stringifyItem(const TempData& value) override;
};

} // namespace storage
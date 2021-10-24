#pragma once

#include <memory>
#include <string>

namespace storage {

class IDataModel {
public:
    using Ptr = std::shared_ptr<IDataModel>;

    virtual ~IDataModel() = default;

    [[nodiscard]] virtual bool
    parse(const std::string& input)
        = 0;

    [[nodiscard]] virtual std::string
    stringify() const = 0;

    template<typename T>
    T&
    castTo()
    {
        return dynamic_cast<T&>(*this);
    }

    template<typename T>
    const T&
    castTo() const
    {
        return dynamic_cast<const T&>(*this);
    }

    template<typename T>
    [[nodiscard]] bool
    typeIs() const
    {
        return (dynamic_cast<const T*>(this) != nullptr);
    }
};

} // namespace storage
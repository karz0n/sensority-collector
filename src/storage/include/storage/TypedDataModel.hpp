#pragma once

#include <Poco/JSON/Parser.h>

#include "storage/IDataModel.hpp"

#include <vector>

namespace storage {

template<typename T, typename Container = std::vector<T>>
class TypedDataModel : public IDataModel {
public:
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using allocator_type = typename Container::allocator_type;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;

    [[nodiscard]] T&
    operator[](std::size_t index)
    {
        return _values[index];
    }

    [[nodiscard]] const T&
    operator[](std::size_t index) const
    {
        return _values[index];
    }

    void
    add(const T& data)
    {
        _values.push_back(data);
    }

    void
    add(T&& data)
    {
        _values.push_back(std::move(data));
    }

    template<typename... Args>
    reference
    emplace(Args&&... args)
    {
        return _values.template emplace_back(std::forward<Args>(args)...);
    }

    [[nodiscard]] std::size_t
    size() const noexcept
    {
        return _values.size();
    }

    void
    clear() noexcept
    {
        _values.clear();
    }

    [[nodiscard]] iterator
    begin() noexcept
    {
        return _values.begin();
    }

    [[nodiscard]] const_iterator
    begin() const noexcept
    {
        return _values.begin();
    }

    [[nodiscard]] iterator
    end() noexcept
    {
        return _values.end();
    }

    [[nodiscard]] const_iterator
    end() const noexcept
    {
        return _values.end();
    }

protected:
    [[nodiscard]] bool
    parseArray(const std::string& input);

    [[nodiscard]] std::string
    stringifyArray() const;

    [[nodiscard]] virtual bool
    parseItem(Poco::JSON::Object::Ptr object, T& value)
        = 0;

    [[nodiscard]] virtual Poco::JSON::Object::Ptr
    stringifyItem(const T& value)
        = 0;

private:
    Container _values;
};

template<typename T, typename Container>
bool
TypedDataModel<T, Container>::parseArray(const std::string& input)
{
    clear();

    Poco::JSON::Parser parser;
    const auto var = parser.parse(input);
    if (!var.isArray()) {
        return false;
    }

    if (auto array = var.extract<Poco::JSON::Array::Ptr>(); array) {
        T data;
        for (std::size_t i = 0; i < array->size(); ++i) {
            if (auto object = array->getObject(i); object) {
                if (parseItem(object, data)) {
                    add(std::move(data));
                }
            }
        }
        return true;
    }

    return false;
}

template<typename T, typename Container>
std::string
TypedDataModel<T, Container>::stringifyArray() const
{
    return {};
}

} // namespace storage
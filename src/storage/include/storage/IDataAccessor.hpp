#pragma once

#include "storage/IDataModel.hpp"

#include <memory>
#include <functional>

namespace storage {

class IDataAccessor {
public:
    using Ptr = std::shared_ptr<IDataAccessor>;

    virtual ~IDataAccessor() = default;

    using GetCallbackSignature = void(IDataModel::Ptr, bool success);
    using PutCallbackSignature = void(bool success);
    using GetCallback = std::function<GetCallbackSignature>;
    using PutCallback = std::function<PutCallbackSignature>;

    virtual void
    put(const std::string& input, PutCallback callback)
        = 0;

    virtual void
    get(int64_t from, int64_t to, GetCallback callback)
        = 0;

    virtual void
    getForLastDay(GetCallback callback)
        = 0;

    virtual void
    getForLastWeek(GetCallback callback)
        = 0;

    virtual void
    getForLastMonth(GetCallback callback)
        = 0;
};

} // namespace storage
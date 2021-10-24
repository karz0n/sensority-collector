#pragma once

#include "storage/IDataModel.hpp"

#include <Poco/Data/Session.h>

#include <memory>
#include <functional>

namespace storage {

class DataAccessor {
public:
    using Ptr = std::shared_ptr<DataAccessor>;

    using GetCallbackSignature = void(IDataModel::Ptr, bool success);
    using PutCallbackSignature = void(bool success);
    using GetCallback = std::function<GetCallbackSignature>;
    using PutCallback = std::function<PutCallbackSignature>;

    DataAccessor() = default;

    virtual ~DataAccessor() = default;

    virtual void
    put(const std::string& input, PutCallback callback)
        = 0;

    virtual void
    get(int64_t from, int64_t to, GetCallback callback)
        = 0;

    void
    getForLastDay(GetCallback callback);

    void
    getForLastWeek(GetCallback callback);

    void
    getForLastMonth(GetCallback callback);

private:
    DataAccessor(const DataAccessor&) = default;
    DataAccessor&
    operator=(const DataAccessor&)
        = default;
};

} // namespace storage
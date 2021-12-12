#pragma once

#include "storage/IDataStorage.hpp"
#include "storage/IDataAccessor.hpp"

#include <memory>
#include <functional>

namespace storage {

class DataAccessor : public IDataAccessor {
public:
    explicit DataAccessor(IDataStorage::Ptr storage);

    void
    put(const std::string& input, PutCallback callback) override;

    void
    get(int64_t from, int64_t to, GetCallback callback) override;

    void
    getForLastDay(GetCallback callback) override;

    void
    getForLastWeek(GetCallback callback) override;

    void
    getForLastMonth(GetCallback callback) override;

protected:
    virtual IDataJob::Ptr
    makePutJob(const std::string& input, PutCallback callback)
        = 0;

    virtual IDataJob::Ptr
    makeGetJob(int64_t from, int64_t to, GetCallback callback)
        = 0;

private:
    IDataStorage::Ptr _storage;
};

} // namespace storage
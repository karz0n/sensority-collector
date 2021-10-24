#pragma once

#include "storage/IDataJob.hpp"
#include "storage/DataAccessor.hpp"
#include "storage/IDataStorage.hpp"

namespace storage {

class GasDataAccessor final : public DataAccessor {
public:
    explicit GasDataAccessor(IDataStorage::Ptr storage);

    void
    put(const std::string& input, PutCallback callback) override;

    void
    get(int64_t from, int64_t to, GetCallback callback) override;

private:
    class GetDataJob : public IDataJob {
    public:
        explicit GetDataJob(int64_t from, int64_t to, GetCallback callback);

        void
        run(Poco::Data::Session& session) override;

    private:
        int64_t _from;
        int64_t _to;
        GetCallback _callback;
    };

    struct PutDataJob : public IDataJob {
    public:
        PutDataJob(IDataModel::Ptr model, PutCallback callback);

        void
        run(Poco::Data::Session& session) override;

    private:
        IDataModel::Ptr _model;
        PutCallback _callback;
    };

private:
    IDataStorage::Ptr _storage;
};

} // namespace storage
#pragma once

#include "storage/IDataJob.hpp"
#include "storage/DataAccessor.hpp"
#include "storage/IDataStorage.hpp"

namespace storage {

class PressureDataAccessor final : public DataAccessor {
public:
    explicit PressureDataAccessor(IDataStorage::Ptr storage);

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
    IDataJob::Ptr
    makePutJob(const std::string& input, PutCallback callback) override;

    IDataJob::Ptr
    makeGetJob(int64_t from, int64_t to, GetCallback callback) override;
};

} // namespace storage
#include "storage/accessor/eCo2DataAccessor.hpp"

#include "common/Logger.hpp"
#include "common/Utils.hpp"
#include "storage/model/eCo2DataModel.hpp"

#include <Poco/Data/Transaction.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Transaction;

using namespace Poco::Data::Keywords;

namespace storage {

eCo2DataAccessor::eCo2DataAccessor(IDataStorage::Ptr storage)
    : _storage{std::move(storage)}
{
}

void
eCo2DataAccessor::put(const std::string& input, PutCallback callback)
{
    try {
        if (auto model = std::make_unique<eCo2DataModel>(); model->parse(input)) {
            auto job = std::make_unique<PutDataJob>(std::move(model), std::move(callback));
            _storage->process(std::move(job));
        } else {
            LOG_ERROR("Failed to parse data");
            invokeIf(callback, false);
        }
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.displayText());
        invokeIf(callback, false);
    }
}

void
eCo2DataAccessor::get(int64_t from, int64_t to, GetCallback callback)
{
    _storage->process(std::make_unique<GetDataJob>(from, to, std::move(callback)));
}

eCo2DataAccessor::GetDataJob::GetDataJob(int64_t from, int64_t to, GetCallback callback)
    : _from{from}
    , _to{to}
    , _callback{std::move(callback)}
{
}

void
eCo2DataAccessor::GetDataJob::run(Poco::Data::Session& session)
{
    static const auto SqlTemplate{"SELECT Equivalent,Accuracy,Timestamp FROM eCo2Data "
                                  "WHERE Timestamp>=%ld AND Timestamp<=%ld ORDER BY Timestamp"};

    try {
        Statement select{session};

        eCo2Data data;
        // clang-format off
        select << Poco::format(SqlTemplate, _from, _to),
            into(data.equivalent),
            into(data.accuracy),
            into(data.timestamp),
            range(0, 1);
        // clang-format on

        auto model = std::make_shared<eCo2DataModel>();
        while (!select.done()) {
            if (select.execute() > 0) {
                model->add(data);
            }
        }

        invokeIf(_callback, std::move(model), true);
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.what());
        invokeIf(_callback, IDataModel::Ptr{}, false);
    }
}

eCo2DataAccessor::PutDataJob::PutDataJob(IDataModel::Ptr model, PutCallback callback)
    : _model{std::move(model)}
    , _callback{std::move(callback)}
{
}

void
eCo2DataAccessor::PutDataJob::run(Session& session)
{
    static const auto SqlTemplate = "INSERT INTO eCo2Data VALUES (?,?,?)";

    try {
        Transaction transaction{session, true};
        poco_assert_dbg(_model->typeIs<eCo2DataModel>());
        auto& model = _model->castTo<eCo2DataModel>();
        for (auto&& value : model) {
            // clang-format off
            session << SqlTemplate,
                use(value.equivalent),
                use(value.accuracy),
                use(value.timestamp),
                now;
            // clang-format on
        }
        transaction.commit();
        invokeIf(_callback, true);
    } catch (const Poco::Exception& e) {
        LOG_ERROR(e.what());
        invokeIf(_callback, false);
    }
}

} // namespace storage
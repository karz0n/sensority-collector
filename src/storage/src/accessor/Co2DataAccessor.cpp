#include "storage/accessor/Co2DataAccessor.hpp"

#include "common/Utils.hpp"
#include "common/Logger.hpp"
#include "storage/model/Co2DataModel.hpp"

#include <Poco/Data/Transaction.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Transaction;

using namespace Poco::Data::Keywords;

namespace storage {

Co2DataAccessor::Co2DataAccessor(IDataStorage::Ptr storage)
    : DataAccessor{std::move(storage)}
{
}

IDataJob::Ptr
Co2DataAccessor::makePutJob(const std::string& input, PutCallback callback)
{
    if (auto model = std::make_unique<Co2DataModel>(); model->parse(input)) {
        return std::make_unique<PutDataJob>(std::move(model), std::move(callback));
    } else {
        throw std::runtime_error{"Failed to parse input for Co2DataModel"};
    }
}

IDataJob::Ptr
Co2DataAccessor::makeGetJob(int64_t from, int64_t to, GetCallback callback)
{
    return std::make_unique<GetDataJob>(from, to, std::move(callback));
}

Co2DataAccessor::GetDataJob::GetDataJob(int64_t from, int64_t to, GetCallback callback)
    : _from{from}
    , _to{to}
    , _callback{std::move(callback)}
{
}

void
Co2DataAccessor::GetDataJob::run(Poco::Data::Session& session)
{
    static const auto SqlTemplate{"SELECT Equivalent,Accuracy,Timestamp FROM Co2Data "
                                  "WHERE Timestamp>=%ld AND Timestamp<=%ld ORDER BY Timestamp"};

    try {
        Statement select{session};

        Co2Data data;
        // clang-format off
        select << Poco::format(SqlTemplate, _from, _to),
            into(data.equivalent),
            into(data.accuracy),
            into(data.timestamp),
            range(0, 1);
        // clang-format on

        auto model = std::make_shared<Co2DataModel>();
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

Co2DataAccessor::PutDataJob::PutDataJob(IDataModel::Ptr model, PutCallback callback)
    : _model{std::move(model)}
    , _callback{std::move(callback)}
{
}

void
Co2DataAccessor::PutDataJob::run(Session& session)
{
    static const auto SqlTemplate = "INSERT INTO Co2Data VALUES (?,?,?)";

    try {
        Transaction transaction{session, true};
        poco_assert_dbg(_model->typeIs<Co2DataModel>());
        auto& model = _model->castTo<Co2DataModel>();
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
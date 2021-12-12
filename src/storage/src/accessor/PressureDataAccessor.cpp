#include "storage/accessor/PressureDataAccessor.hpp"

#include "common/Utils.hpp"
#include "common/Logger.hpp"
#include "storage/model/PressureDataModel.hpp"

#include <Poco/Data/Transaction.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Transaction;

using namespace Poco::Data::Keywords;

namespace storage {

PressureDataAccessor::PressureDataAccessor(IDataStorage::Ptr storage)
    : DataAccessor{std::move(storage)}
{
}

IDataJob::Ptr
PressureDataAccessor::makePutJob(const std::string& input, PutCallback callback)
{
    if (auto model = std::make_unique<PressureDataModel>(); model->parse(input)) {
        return std::make_unique<PutDataJob>(std::move(model), std::move(callback));
    } else {
        throw std::runtime_error{"Failed to parse input for PressureDataModel"};
    }
}

IDataJob::Ptr
PressureDataAccessor::makeGetJob(int64_t from, int64_t to, GetCallback callback)
{
    return std::make_unique<GetDataJob>(from, to, std::move(callback));
}

PressureDataAccessor::GetDataJob::GetDataJob(int64_t from, int64_t to, GetCallback callback)
    : _from{from}
    , _to{to}
    , _callback{std::move(callback)}
{
}

void
PressureDataAccessor::GetDataJob::run(Poco::Data::Session& session)
{
    static const auto SqlTemplate{"SELECT Value,Timestamp FROM PressureData "
                                  "WHERE Timestamp>=%ld AND Timestamp<=%ld ORDER BY Timestamp"};

    try {
        Statement select{session};

        PressureData data;
        // clang-format off
        select << Poco::format(SqlTemplate, _from, _to),
            into(data.value),
            into(data.timestamp),
            range(0, 1);
        // clang-format on

        auto model = std::make_shared<PressureDataModel>();
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

PressureDataAccessor::PutDataJob::PutDataJob(IDataModel::Ptr model, PutCallback callback)
    : _model{std::move(model)}
    , _callback{std::move(callback)}
{
}

void
PressureDataAccessor::PutDataJob::run(Session& session)
{
    static const auto SqlTemplate = "INSERT INTO PressureData VALUES (?,?)";

    try {
        Transaction transaction{session, true};
        poco_assert_dbg(_model->typeIs<PressureDataModel>());
        auto& model = _model->castTo<PressureDataModel>();
        for (auto&& value : model) {
            // clang-format off
            session << SqlTemplate,
                use(value.value),
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
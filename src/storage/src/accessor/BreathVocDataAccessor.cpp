#include "storage/accessor/BreathVocDataAccessor.hpp"

#include "common/Utils.hpp"
#include "common/Logger.hpp"
#include "storage/model/BreathVocDataModel.hpp"

#include <Poco/Data/Transaction.h>
#include <Poco/DateTime.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::Transaction;

using namespace Poco::Data::Keywords;

namespace storage {

BreathVocDataAccessor::BreathVocDataAccessor(IDataStorage::Ptr storage)
    : DataAccessor{std::move(storage)}
{
}

IDataJob::Ptr
BreathVocDataAccessor::makePutJob(const std::string& input, PutCallback callback)
{
    if (auto model = std::make_unique<BreathVocDataModel>(); model->parse(input)) {
        return std::make_unique<PutDataJob>(std::move(model), std::move(callback));
    } else {
        throw std::runtime_error{"Failed to parse input for BreathVocDataModel"};
    }
}

IDataJob::Ptr
BreathVocDataAccessor::makeGetJob(int64_t from, int64_t to, GetCallback callback)
{
    return std::make_unique<GetDataJob>(from, to, std::move(callback));
}

BreathVocDataAccessor::GetDataJob::GetDataJob(int64_t from, int64_t to, GetCallback callback)
    : _from{from}
    , _to{to}
    , _callback{std::move(callback)}
{
}

void
BreathVocDataAccessor::GetDataJob::run(Poco::Data::Session& session)
{
    static const auto SqlTemplate{"SELECT Equivalent,Accuracy,Timestamp FROM BreathVocData "
                                  "WHERE Timestamp>=%ld AND Timestamp<=%ld ORDER BY Timestamp"};

    try {
        Statement select{session};

        BreathVocData data;
        // clang-format off
        select << Poco::format(SqlTemplate, _from, _to),
            into(data.equivalent),
            into(data.accuracy),
            into(data.timestamp),
            range(0, 1);
        // clang-format on

        auto model = std::make_shared<BreathVocDataModel>();
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

BreathVocDataAccessor::PutDataJob::PutDataJob(IDataModel::Ptr model, PutCallback callback)
    : _model{std::move(model)}
    , _callback{std::move(callback)}
{
}

void
BreathVocDataAccessor::PutDataJob::run(Session& session)
{
    static const auto SqlTemplate = "INSERT INTO BreathVocData VALUES (?,?,?)";

    try {
        Transaction transaction{session, true};
        poco_assert_dbg(_model->typeIs<BreathVocDataModel>());
        auto& model = _model->castTo<BreathVocDataModel>();
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
#pragma once

#include <Poco/Data/Session.h>

#include <memory>

namespace storage {

class IDataAccessor {
public:
    using Ptr = std::shared_ptr<IDataAccessor>;

    IDataAccessor() = default;

    virtual ~IDataAccessor() = default;

    virtual void
    access(Poco::Data::Session& session)
        = 0;

private:
    IDataAccessor(const IDataAccessor&) = default;
    IDataAccessor&
    operator=(const IDataAccessor&)
        = default;
};

} // namespace storage
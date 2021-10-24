#pragma once

#include <Poco/Data/Session.h>

#include <memory>

namespace storage {

class IDataJob {
public:
    using Ptr = std::unique_ptr<IDataJob>;

    virtual ~IDataJob() = default;

    virtual void
    run(Poco::Data::Session& session)
        = 0;
};

} // namespace storage
#pragma once

#include "storage/IDataStorage.hpp"

#include <Poco/Data/Session.h>

#include <queue>
#include <thread>
#include <atomic>

namespace storage {

class DataStorage final : public IDataStorage {
public:
    explicit DataStorage(const std::string& connectionString = ":memory:");

    ~DataStorage() override;

    void
    initialize() override;

    void
    uninitialize() override;

    std::future<bool>
    process(IDataAccessor::Ptr accessor) override;

    static void
    setUp();

    static void
    tearDown();

private:
    struct Task {
        explicit Task(IDataAccessor::Ptr accessor)
            : accessor{std::move(accessor)}
        {
        }

        IDataAccessor::Ptr accessor;
        std::promise<bool> done;
    };

    using Tasks = std::queue<Task>;

private:
    void
    loop();

    void
    terminate();

private:
    Poco::Data::Session _session;
    Tasks _tasks;
    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _terminate;
};

} // namespace storage
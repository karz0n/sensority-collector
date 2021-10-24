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
    setUp() override;

    void
    tearDown() override;

    void
    process(IDataJob::Ptr job) override;

    static void
    initialize();

    static void
    uninitialize();

private:
    void
    loop();

    void
    terminate();

private:
    Poco::Data::Session _session;
    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _active;
    std::queue<IDataJob::Ptr> _jobs;
};

} // namespace storage
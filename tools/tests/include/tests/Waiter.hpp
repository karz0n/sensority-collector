#pragma once

class Waiter final {
public:
    Waiter() = default;

    ~Waiter()
    {
        notifyAll();
    }

    template<typename Rep, typename Per>
    bool
    waitFor(std::chrono::duration<Rep, Per> time)
    {
        std::unique_lock lock{m_mutex};
        return (m_cv.wait_for(lock, time) != std::cv_status::timeout);
    }

    template<typename Rep, typename Per, typename Predicate>
    bool
    waitFor(std::chrono::duration<Rep, Per> time, Predicate predicate)
    {
        std::unique_lock lock{m_mutex};
        return m_cv.wait_for(lock, time, std::move(predicate));
    }

    void
    notifyOne()
    {
        m_cv.notify_one();
    }

    void
    notifyAll()
    {
        m_cv.notify_all();
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
};
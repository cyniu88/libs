#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <optional>
#include <type_traits>
#include <stdexcept>

class ThreadPool {
public:
    enum class EnqueueMode {
        Blocking,
        NonBlocking
    };

    explicit ThreadPool(size_t threads = std::thread::hardware_concurrency(),
                        size_t maxQueueSize = 100,
                        EnqueueMode mode = EnqueueMode::Blocking);
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::optional<std::future<typename std::invoke_result_t<F, Args...>>> 
    {
        using return_type = typename std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (mode == EnqueueMode::Blocking) {
                queueNotFull.wait(lock, [this] {
                    return stopFlag || tasks.size() < maxQueueSize;
                });
            } else {
                if (tasks.size() >= maxQueueSize || stopFlag)
                    return std::nullopt; // kolejka pełna — odrzucamy
            }

            if (stopFlag)
                return std::nullopt;

            tasks.emplace([task]() { (*task)(); });
        }

        condition.notify_one();
        return res;
    }

    void stop();

private:
    void workerLoop();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable queueNotFull;
    std::atomic<bool> stopFlag{false};

    size_t maxQueueSize;
    EnqueueMode mode;
};

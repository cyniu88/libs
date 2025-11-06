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
#include <string>
#include <random>
#include <iostream>

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
        return enqueue("", std::forward<F>(f), std::forward<Args>(args)...);
    }

    template<class F, class... Args>
    auto enqueue(const std::string& threadName, F&& f, Args&&... args)
        -> std::optional<std::future<typename std::invoke_result_t<F, Args...>>> 
    {
        using return_type = typename std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        std::string name = threadName.empty() ? generateRandomName() : threadName;

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (mode == EnqueueMode::Blocking) {
                queueNotFull.wait(lock, [this] {
                    return stopFlag || tasks.size() < maxQueueSize;
                });
            } else {
                if (tasks.size() >= maxQueueSize || stopFlag)
                    return std::nullopt;
            }

            if (stopFlag)
                return std::nullopt;

            tasks.emplace([this, task, name]() {
                setThreadName(name);
                (*task)();
            });
        }

        condition.notify_one();
        return res;
    }

    void stop();
    std::string printThreadNames() const;

private:
    void workerLoop(size_t workerId);
    void setThreadName(const std::string& name);
    std::string generateRandomName() const;

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    mutable std::vector<std::string> threadNames;

    std::mutex queueMutex;
    mutable std::mutex namesMutex;
    std::condition_variable condition;
    std::condition_variable queueNotFull;
    std::atomic<bool> stopFlag{false};

    size_t maxQueueSize;
    EnqueueMode mode;
};

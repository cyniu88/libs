#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threads, size_t maxQueueSize, EnqueueMode mode)
    : maxQueueSize(maxQueueSize), mode(mode)
{
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag = true;
    }

    condition.notify_all();
    queueNotFull.notify_all();

    for (auto &worker : workers)
        if (worker.joinable())
            worker.join();
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] {
                return stopFlag || !tasks.empty();
            });

            if (stopFlag && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
            queueNotFull.notify_one();
        }

        task();
    }
}

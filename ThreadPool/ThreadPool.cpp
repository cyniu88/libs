#include "ThreadPool.h"

#include <sstream>

ThreadPool::ThreadPool(size_t threads, size_t maxQueueSize, EnqueueMode mode)
    : threadNames(threads, "NULL"), maxQueueSize(maxQueueSize), mode(mode)
{
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this, i);
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

void ThreadPool::workerLoop(size_t workerId) {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] {
                return stopFlag || !tasks.empty();
            });

            if (stopFlag && tasks.empty()) {
                std::lock_guard<std::mutex> nameLock(namesMutex);
                threadNames[workerId] = "NULL";
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
            queueNotFull.notify_one();
        }

        task();
        
        {
            std::lock_guard<std::mutex> nameLock(namesMutex);
            threadNames[workerId] = "NULL";
        }
    }
}

void ThreadPool::setThreadName(const std::string& name) {
    std::thread::id currentId = std::this_thread::get_id();
    std::lock_guard<std::mutex> lock(namesMutex);
    
    for (size_t i = 0; i < workers.size(); ++i) {
        if (workers[i].get_id() == currentId) {
            threadNames[i] = name;
            break;
        }
    }
}

std::string ThreadPool::generateRandomName() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    return "Task_" + std::to_string(dis(gen));
}

std::string ThreadPool::printThreadNames() const {
    std::lock_guard<std::mutex> lock(namesMutex);
    std::stringstream ss;
    ss << "Thread names: ";
    for (size_t i = 0; i < threadNames.size(); ++i) {
        ss << "[" << i << ": " << threadNames[i] << "]" << std::endl;
        if (i < threadNames.size() - 1) ss << std::endl;
    }
    return ss.str();
}

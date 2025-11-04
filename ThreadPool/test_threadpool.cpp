#include <gtest/gtest.h>
#include "ThreadPool.h"
#include <chrono>
#include <atomic>

class ThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ThreadPoolTest, BasicExecution) {
    ThreadPool pool(2, 10);
    std::atomic<int> counter{0};
    
    auto future = pool.enqueue([&counter]() {
        counter++;
        return 42;
    });
    
    ASSERT_TRUE(future.has_value());
    EXPECT_EQ(future->get(), 42);
    EXPECT_EQ(counter.load(), 1);
}

TEST_F(ThreadPoolTest, MultipleTasksExecution) {
    ThreadPool pool(3, 10);
    std::atomic<int> counter{0};
    
    std::vector<std::optional<std::future<void>>> futures;
    for (int i = 0; i < 5; ++i) {
        futures.push_back(pool.enqueue([&counter]() {
            counter++;
        }));
    }
    
    for (auto& future : futures) {
        ASSERT_TRUE(future.has_value());
        future->get();
    }
    
    EXPECT_EQ(counter.load(), 5);
}

TEST_F(ThreadPoolTest, NonBlockingModeRejectsWhenFull) {
    ThreadPool pool(1, 1, ThreadPool::EnqueueMode::NonBlocking);
    
    // Blokuj jedyny wątek długim zadaniem
    auto blocking_future = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Pozwól wątkowi zacząć
    
    // Wypełnij kolejkę
    auto queued_future = pool.enqueue([]() {});
    
    // Następne zadanie powinno być odrzucone
    auto rejected_future = pool.enqueue([]() {});
    
    ASSERT_TRUE(blocking_future.has_value());
    ASSERT_TRUE(queued_future.has_value());
    EXPECT_FALSE(rejected_future.has_value());
}

TEST_F(ThreadPoolTest, TasksWithArguments) {
    ThreadPool pool(2, 10);
    
    auto future = pool.enqueue([](int a, int b) {
        return a + b;
    }, 3, 4);
    
    ASSERT_TRUE(future.has_value());
    EXPECT_EQ(future->get(), 7);
}

TEST_F(ThreadPoolTest, StopPreventsNewTasks) {
    ThreadPool pool(2, 10);
    pool.stop();
    
    auto future = pool.enqueue([]() { return 1; });
    EXPECT_FALSE(future.has_value());
}
#include "ThreadPool.h"
#include <iostream>
#include <chrono>

int main() {
    ThreadPool pool(3, 3, ThreadPool::EnqueueMode::Blocking);

    std::mutex mtx;
    auto boo = [&mtx] (int i) {
            if (i == 5) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Zadanie " << i << " wykonane w wątku: "
                      << std::this_thread::get_id() << std::endl;
        };


    for (int i = 0; i < 100; ++i) {
        auto result = pool.enqueue(boo, i);

        if (!result)
            std::cout << "❌ Zadanie " << i << " odrzucone (kolejka pełna)\n";
    }

    std::cout << "Wszystkie zadania zostały zlecone.\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    pool.stop();
    std::cout << "Wszystkie zadania zostały wykonane.\n";
}

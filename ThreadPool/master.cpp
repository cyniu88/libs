#include "ThreadPool.h"
#include <iostream>
#include <chrono>

int main() {
    ThreadPool pool(3, 3, ThreadPool::EnqueueMode::Blocking);

    for (int i = 0; i < 10; ++i) {
        auto result = pool.enqueue([i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "Zadanie " << i << " wykonane w wątku: "
                      << std::this_thread::get_id() << std::endl;
        });

        if (!result)
            std::cout << "❌ Zadanie " << i << " odrzucone (kolejka pełna)\n";
    }

    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    pool.stop();
}

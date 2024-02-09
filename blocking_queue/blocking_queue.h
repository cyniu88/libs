#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <assert.h>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <vector>

template<class T>
class BlockingQueue{
public:

    BlockingQueue() :mtx(), full_(), empty_(), capacity_(MAX_CAPACITY) { }
    void Put(const T& task){
        std::unique_lock<std::mutex> lock(mtx);
        if (queue_.size() > capacity_ - 2){

            return;
        }
        full_.wait(lock, [this]{return (queue_.size() < capacity_);} );
        //assert(queue_.size() < capacity_);
        queue_.push(task);
        empty_.notify_all();
    }

    T Take(){
        std::unique_lock<std::mutex> lock(mtx);
        empty_.wait(lock, [this]{return !queue_.empty();} );
        assert(!queue_.empty());
        T front(queue_.front());
        queue_.pop();
        full_.notify_all();
        return front;
    }

    size_t Size(){
        std::lock_guard<std::mutex> lock(mtx);
        return queue_.size();
    }

    T Front(){
        std::unique_lock<std::mutex> lock(mtx);
        empty_.wait(lock, [this]{return !queue_.empty();} );
        assert(!queue_.empty());
        T front(queue_.front());
        return front;
    }

    T Back(){
        std::unique_lock<std::mutex> lock(mtx);
        empty_.wait(lock, [this]{return !queue_.empty();} );
        assert(!queue_.empty());
        T back(queue_.back());
        return back;
    }

    bool Empty(){
        std::unique_lock<std::mutex> lock(mtx);
        return queue_.empty();
    }

    void SetCapacity(const size_t capacity){
        capacity_ = (capacity > 0 ? capacity : MAX_CAPACITY);
    }

private:
    BlockingQueue(const BlockingQueue& rhs);
    BlockingQueue& operator= (const BlockingQueue& rhs);

private:
    mutable std::mutex mtx;
    std::condition_variable full_;
    std::condition_variable empty_;
    std::queue<T> queue_;
    size_t capacity_;
    static constexpr int MAX_CAPACITY = 200;
};

#endif

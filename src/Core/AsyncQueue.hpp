/** @file AsyncQueue.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Wrapper for classic std::queue to work with multithreading and sleeping
 *
 *  ### Contributors
 *  kaufmlu1
 *  veselo21
 *  zimaluk1
 *  rehorja8
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * Wrapper around queue to make it thread safe
 * @tparam T stored value type
 */
template<typename T>
class AsyncQueue {
public:
    /**
     * Constructor for when it has to use it's own mutex
     */
    AsyncQueue() { mutex = &own_mutex; }
    
    /**
     * Constructor with an external mutex
     * @param provided_mutex external mutex
     */
    explicit AsyncQueue(std::mutex& provided_mutex) { mutex = &provided_mutex; }
    
    /**
     * Used when assigning external mutex after creation
     * @param provided_mutex external mutex
     */
    void SetMutex(std::mutex& provided_mutex) { mutex = &provided_mutex; }
    
    /**
     * Adds an item to the queue
     * @param item added item
     */
    void Push(T&& item) {
        std::lock_guard<std::mutex> lock(*mutex);
        queue.push(std::move(item));
        condition.notify_one();
    }
    
    /**
     * Adds an item to the queue via copy - for backwards compatibility
     * @param item added item
     */
    void Push(const T& item) {
        this->Push(T(item));
    }
    
    /**
     * Returns the front item of the queue
     * @return front item
     */
    T Peek() {
        std::unique_lock<std::mutex> lock(*mutex);
        while (queue.empty())
            condition.wait(lock);
        
        T item = queue.front();
        return item;
    }
    
    /**
     * Removes and returns an item from the front of the queue
     * @return front item
     */
    T Pop() {
        std::unique_lock<std::mutex> lock(*mutex);
        while (queue.empty())
            condition.wait(lock);
        
        T item = std::move(queue.front());
        queue.pop();
        return item;
    }

private:
    std::condition_variable condition;
    std::mutex own_mutex;
    std::mutex* mutex;
    std::queue<T> queue;
};

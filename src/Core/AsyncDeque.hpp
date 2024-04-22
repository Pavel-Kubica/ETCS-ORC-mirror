/** @file AsyncDeque.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Wrapper for classic std::deque to work with multithreading and sleeping
 *
 *  ### Contributors
 *  kaufmlu1
 *  zimaluk1
 */

#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

#include "nlohmann/json.hpp"
/**
 * Wrapper around deque to make it thread safe
 * @tparam T stored value type
 */
template <typename T>
class AsyncDeque {
public:
    /**
     * Constructor for when it has to use it's own mutex
     */
    AsyncDeque() { mutex = &own_mutex; }
    /**
     * Constructor with an external mutex
     * @param provided_mutex external mutex
     */
    explicit AsyncDeque(std::mutex& provided_mutex) { mutex = &provided_mutex; }
    /**
     * Used when assigning external mutex after creation
     * @param provided_mutex external mutex
     */
    void SetMutex(std::mutex& provided_mutex) { mutex = &provided_mutex; }

    /**
     * Adds an item to the back of the deque
     * @param item added item
     */
    void PushBack(T item) {
        std::lock_guard<std::mutex> lock(*mutex);
        deque.push_back(item);
        condition.notify_one();
    }

    /**
     * Adds an item to the front of the deque
     * @param item added item
     */
    void PushFront(T item) {
        std::lock_guard<std::mutex> lock(*mutex);
        deque.push_front(item);
        condition.notify_one();
    }
    /**
     * Returns the front item of the deque
     * @return front item
     */
    T Front() {
        std::unique_lock<std::mutex> lock(*mutex);
        while (deque.empty())
            condition.wait(lock);

        T item = deque.front();
        return item;
    }

    /**
     * Returns the back item of the deque
     * @return back item
     */
    T Back() {
        std::unique_lock<std::mutex> lock(*mutex);
        while (deque.empty())
            condition.wait(lock);

        T item = deque.back();
        return item;
    }
    /**
     * Removes and returns an item from the front of the deque
     * @return front item
     */
    T PopFront() {
        T item = Front();
        deque.pop_front();
        return item;
    }

    /**
     * Removes and returns an item from the front of the deque
     * @return front item
     */
    T PopBack() {
        T item = Back();
        deque.pop_back();
        return item;
    }

    /**
     * Returns whether the deque is empty
     * @return is empty?
     */
    bool Empty() {
        std::lock_guard<std::mutex> lock(*mutex);
        return deque.empty();
    }

    /**
     * Clears the deque
     */
    void Clear() {
        std::lock_guard<std::mutex> lock(*mutex);
        deque.clear();
    }
    /**
     * Returns a nlohmann::json object of the async deque
     * @return nlohmann::json object
     */
    nlohmann::json to_json() {
        std::lock_guard<std::mutex> lock(*mutex);
        return nlohmann::json(deque);
    }
    /**
     * Returns a copy of the deque
     * @return copy of the deque
     */
    std::deque<T> GetAll() {
        std::lock_guard<std::mutex> lock(*mutex);
        std::deque<T> all;
        for (auto i : deque) {
            all.emplace_back(i);
        }
        return all;
    }
    /**
     * Returns a vector with the deque elements
     * @return vector with all the deque elements
     */
    std::vector<T> GetVector() {
        std::lock_guard<std::mutex> lock(*mutex);
        std::vector<T> all;
        for (auto i : deque) {
            all.emplace_back(i);
        }
        return all;
    }

private:
    std::condition_variable condition;
    std::mutex own_mutex;
    std::mutex* mutex;
    std::deque<T> deque;
};

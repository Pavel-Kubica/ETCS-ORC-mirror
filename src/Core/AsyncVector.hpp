/** @file AsyncVector.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Wrapper for classic std::vector to work with multithreading and sleeping
 *
 *  ### Contributors
 *  caslama1
 */

#pragma once

#include <functional>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Class AsyncVector is thread safe implementation of std::vector.
 * @tparam T stored value type.
 */
template <typename T>
class AsyncVector {
public:
    /**
     * Default constructor.
     * Initializes the vector to an empty state.
     */
    AsyncVector() {
        std::lock_guard<std::mutex> lock(mutex);
        vector = {};
    }

    /**
     * Constructor with provided vector.
     * Initializes the vector with the contents of the provided vector.
     * @param providedVector The vector to initialize with.
     */
    explicit AsyncVector(const std::vector<T>& providedVector) {
        std::lock_guard<std::mutex> lock(mutex);
        vector = providedVector;
    }

    /* -------------------- MODIFIERS -------------------- */

    /**
     * Method Clear erases all items from the vector. After this call, Size() returns 0.
     */
    void Clear() {
        std::lock_guard<std::mutex> lock(mutex);
        vector.clear();
    }

    /**
     * Method PushBack appends the given item value to the end of the vector.
     * @param value the value of the item to append.
     */
    void PushBack(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        vector.push_back(value);
    }

    /**
     * Method ChangeValue changes value of the specified index with bounds checking.
     * If index is not within the range of the vector, an exception of type std::out_of_range is thrown.
     * @param index index of the item to changed.
     * @param value new item value.
     */
    void ChangeValue(size_t index, const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (index < vector.size()) {
            vector[index] = value;
        } else {
            throw std::out_of_range("Index " + std::to_string(index) + " out of bounds.");
        }
    }

    void Replace(const std::vector<T>& newVector) {
        std::lock_guard<std::mutex> lock(mutex);
        vector = newVector;
    }

    /* -------------------- CAPACITY -------------------- */

    /**
     * Method Size returns the number of items in the vector.
     * @return the number of items in the vector.
     */
    size_t Size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return vector.size();
    }

    /**
     * Method Empty checks if the vector has no items.
     * @return true if the vector is empty, false otherwise.
     */
    bool Empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return vector.empty();
    }

    /* -------------------- ELEMENT ACCESS -------------------- */

    /**
     * Method At returns value of the item at specified index with bounds checking.
     * If index is not within the range of the vector, an exception of type std::out_of_range is thrown.
     * @param index index of the item to return.
     * @return value of the requested item.
     */
    T At(size_t index) const {
        std::lock_guard<std::mutex> lock(mutex);
        if (index < vector.size()) {
            return vector.at(index);
        } else {
            throw std::out_of_range("Index " + std::to_string(index) + " out of bounds.");
        }
    }

    /**
     * Operator [] access item on specified index. No bounds checking is performed.
     * @param index index of the item to return.
     * @return value of the requested item.
     */
    T operator[](size_t index) const {
        std::lock_guard<std::mutex> lock(mutex);
        return vector[index];
    }

    /**
     * Method Front returns a value of the first item in the vector.
     * Calling Front on an empty vector causes undefined behavior.
     * @return value of the first item.
     */
    T Front() const { return At(0); }

    /**
     * Method Back returns a value of the last item in the vector.
     * Calling Back on an empty vector causes undefined behavior.
     * @return value of the last item.
     */
    T Back() const { return At(vector.size() - 1); }

    /**
     * Method Data returns copy of vector, which means, that you will your own copy on which you can perform
     * anything you want.
     * @return copy of vector which has all stored data.
     */
    std::vector<T> Data() const {
        std::lock_guard<std::mutex> lock(mutex);
        return vector;
    }

    /* -------------- OPERATIONS OVER VECTOR --------------- */

    void ForEach(std::function<void(T&)> func) {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto& item : vector) {
            func(item);
        }
    }

    void ForEach(std::function<void(const T&)> func) const {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto& item : vector) {
            func(item);
        }
    }

private:
    std::vector<T> vector;     //! The underlying vector
    mutable std::mutex mutex;  //! Mutex for thread-safety.
};
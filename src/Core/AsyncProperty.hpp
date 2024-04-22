/** @file AsyncProperty.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Wrapper for any object to work with multithreading and sleeping
 *
 *  ### Contributors
 *  kaufmlu1
 *  zimaluk1
 */

#pragma once

#include <mutex>
/**
 * Wrapper around properties to make them thread safe
 * @tparam T property type
 */
template <typename T>
class AsyncProperty {
public:
    /**
     * Constructor for when it has to use it's own mutex
     */
    AsyncProperty() { mutex = &own_mutex; }
    /**
     * Constructor for when it has to use it's own mutex
     * and wrap a value
     * @param value the wrapped value, data
     */
    explicit AsyncProperty(const T& value) {
        mutex = &own_mutex;
        this->value = value;
    }
    /**
     * Constructor for when it has to use a provided mutex
     * @param provided_mutex
     */
    explicit AsyncProperty(std::mutex& provided_mutex) { mutex = &provided_mutex; }
    /**
     * Constructor for when it has to use a provided mutex
     * and wrap a value
     * @param provided_mutex external mutex
     * @param value the wrapped value, data
     */
    AsyncProperty(std::mutex& provided_mutex, const T& value) {
        mutex = &provided_mutex;
        this->value = value;
    }

    /**
     * Used when assigning external mutex after creation
     * @param provided_mutex external mutex
     */
    void SetMutex(std::mutex& provided_mutex) { mutex = &provided_mutex; }
    /**
     *
     * @return reference to this object
     */
    AsyncProperty<T>& GetReference() { return *this; }

    /**
     * Returns the stored value
     * @return stored value
     */
    T GetValue() {
        std::lock_guard<std::mutex> lock(*mutex);
        return value;
    }

    /**
     * Operator that returns the stored value
     * @return stored value
     */
    operator T() const {
        std::lock_guard<std::mutex> lock(*mutex);
        return value;
    }
    /**
     * Operator that sets a new value
     * @param new_value
     * @return this object with the new value
     */
    T operator=(const T& new_value) {
        std::lock_guard<std::mutex> lock(*mutex);
        return this->value = new_value;
    }

private:
    std::mutex own_mutex;
    std::mutex* mutex;
    T value;
};

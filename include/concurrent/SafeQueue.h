#pragma once

#include <mutex>
#include <queue>

/**
 * A simple implementation of thread safe queue.
*/
template<typename T> 
class SafeQueue {
    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;

    public:
        SafeQueue() {}

        bool empty() {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        int size() {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_queue.size();
        }

        void push(T& value) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(value);
        }

        bool pop() {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_queue.empty()) return false;
            m_queue.pop();
            return true;
        }

        T front() {
            std::unique_lock<std::mutex> lock(m_mutex);
            return std::move(m_queue.front());
        }
};
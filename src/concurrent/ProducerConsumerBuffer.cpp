#include "ProducerConsumerBuffer.h"

#include<iostream>

void ProducerConsumerBuffer::Produce(int val, int thread_id) {
    std::unique_lock<std::mutex> lock(m_mtx);
    // Wait if the buffer has content.
    m_no_content.wait(lock, [this]() { return m_buffer.empty(); });

    std::cout << "Producer in thread: " << thread_id << " has produced: " << val << std::endl;
    m_buffer.push(val);
    m_has_content.notify_one();
}

void ProducerConsumerBuffer::Consume(int thread_id) {
    std::unique_lock<std::mutex> lock(m_mtx);
    // Wait if the buffer has no content.
    m_has_content.wait(lock, [this]() { return !m_buffer.empty(); });
    int val = m_buffer.front();
    m_buffer.pop();
    std::cout << "Consumer in thread: " << thread_id << " has consumed: " << val << std::endl;
    m_no_content.notify_one();
}

void ProducerConsumerBuffer::ProducerTest() {
    for (int i = 0; i < 20; ++i) {
        int x = rand();
        Produce(i, x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ProducerConsumerBuffer::ConsumerTest() {
    for (int i = 0; i < 20; ++i) {
        int x = rand();
        Consume(x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
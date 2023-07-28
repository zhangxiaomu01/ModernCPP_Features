#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <chrono>

#include "SafeQueue.h"

class ProducerConsumerBuffer {
private:
    SafeQueue<int> m_buffer;
    std::mutex m_mtx;
    std::condition_variable m_has_content;
    std::condition_variable m_no_content;

public:
    ProducerConsumerBuffer() = default;

    void Produce(int val, int thread_id);

    void Consume(int thread_id);

    // Utility methods to test the producer - consumer mode.
    void ProducerTest();

    void ConsumerTest();
};

/*
// An example usage for the producer / consumer buffer class.
int main() {
    ProducerConsumerBuffer buffer;

    std::thread produceThread0(&ProducerConsumerBuffer::ProducerTest, &buffer);
    std::thread consumerThread0(&ProducerConsumerBuffer::ConsumerTest, &buffer);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    produceThread0.join();
    consumerThread0.join();

    std::cout << "Hello world!" << std::endl;
    return 0;
}
*/
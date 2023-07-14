#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <future>
#include <condition_variable>
#include <memory>

#include "SafeQueue.h"

using task = std::function<void()>;

class SimpleThreadPool{
    private:
        bool m_shutdown;
        SafeQueue<task> m_queue;
        std::vector<std::thread> m_threads;
        std::mutex m_mutex;
        std::condition_variable m_cv;

    public:
        SimpleThreadPool(size_t thread_num);

        void Init();

        void Shutdown();

        std::future<void> Submit(task t);

    private:
        class WorkerThread {
            private:
                int m_id;
                std::unique_ptr<SimpleThreadPool> m_thread_pool;
            
            public:
                WorkerThread(SimpleThreadPool* thread_pool, int id);

                void operator()();
        };
};


/**
 * The following code snipts shows how to use the thread pool.
*/
// void testThreadPool() {
//     SimpleThreadPool thread_pool(4);
//     thread_pool.Init();

//     for (int i = 0; i < 100; ++i) {
//         task newTask = []() {
//             std::cout << "This is a task" << std::endl;
//         };
//         thread_pool.Submit(newTask);
//     }
//     std::cout << "task submited" << std::endl;
//     thread_pool.Shutdown();
// }

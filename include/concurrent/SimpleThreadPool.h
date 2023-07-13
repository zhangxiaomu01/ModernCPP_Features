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

        void Submit(task t);

    private:
        class WorkerThread {
            private:
                int m_id;
                std::unique_ptr<SimpleThreadPool> m_thread_pool;
            
            public:
                WorkerThread(std::unique_ptr<SimpleThreadPool> thread_pool, int id);

                void operator()();
        };
};
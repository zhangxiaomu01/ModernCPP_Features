#include "SimpleThreadPool.h"

#include <memory>

SimpleThreadPool::SimpleThreadPool(size_t thread_num = std::thread::hardware_concurrency()) {
    m_threads = std::vector<std::thread>(thread_num);
    m_shutdown = false;
}

void SimpleThreadPool::Init()
{
    for (int i = 0; i < m_threads.size(); ++i) {
        m_threads[i] = std::thread(SimpleThreadPool::WorkerThread(this, i));
    }
}

void SimpleThreadPool::Shutdown() {
    m_shutdown = true;
    m_cv.notify_all();
    for (int i = 0; i < m_threads.size(); ++i) {
        if (m_threads[i].joinable()) m_threads[i].join();
    }
}

std::future<void> SimpleThreadPool::Submit(task t) {
    std::shared_ptr<std::packaged_task<void()>> task_ptr = 
        std::make_shared<std::packaged_task<void()>>(t);
    task wrapper_task = [task_ptr]() {
        (*task_ptr)();
    };
    m_queue.push(wrapper_task);
    m_cv.notify_one();
    return task_ptr->get_future();
}

SimpleThreadPool::WorkerThread::WorkerThread(SimpleThreadPool* thread_pool, int id)
: m_thread_pool(thread_pool), m_id(id) {}

void SimpleThreadPool::WorkerThread::operator()() {
    task current_task;
    bool pop_succeeded = false;
    while(!m_thread_pool->m_shutdown) {
        std::unique_lock<std::mutex> lock(m_thread_pool->m_mutex);
        if (m_thread_pool->m_queue.empty()) {
            m_thread_pool->m_cv.wait(lock);
        }
        std::cout<<"Thread " << m_id << " is executing task! " << std::endl;
        current_task = m_thread_pool->m_queue.front();
        pop_succeeded = m_thread_pool->m_queue.pop();
        if(pop_succeeded) current_task();
    }
}
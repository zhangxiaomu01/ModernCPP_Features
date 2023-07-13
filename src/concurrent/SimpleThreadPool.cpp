#include "SimpleThreadPool.h"

SimpleThreadPool::SimpleThreadPool(size_t thread_num = std::thread::hardware_concurrency()) {
    m_threads = std::vector<std::thread>(thread_num);
    m_shutdown = false;
}

void SimpleThreadPool::Init()
{

}

void SimpleThreadPool::Shutdown() {

}

void SimpleThreadPool::Submit(task t) {

}

SimpleThreadPool::WorkerThread::WorkerThread(std::unique_ptr<SimpleThreadPool> thread_pool, int id)
: m_thread_pool(std::move(thread_pool)), m_id(id) {}

void SimpleThreadPool::WorkerThread::operator()() {

}
#include<windows.h>
#include<iostream>

#include<SimpleThreadPool.h>

int main() {
    SimpleThreadPool thread_pool(4);
    thread_pool.Init();

    for (int i = 0; i < 100; ++i) {
        task newTask = []() {
            std::cout << "This is a task" << std::endl;
        };
        thread_pool.Submit(newTask);
    }
    std::cout << "task submited" << std::endl;
    Sleep(1000);
    thread_pool.Shutdown();
}

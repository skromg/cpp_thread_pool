//
// Created by swh on 2/21/23.
//

#include <iostream>
#include "threadpool.h"

using std::cout;
using std::endl;
using std::flush;

#define FLUSH flush(cout)

ThreadPool::ThreadPool(size_t thread_num)
{
    InitThreadPool(thread_num);
}

ThreadPool::~ThreadPool()
{
    stop_flag_ = true;
    cond_.notify_all();
    for (auto &item : thread_pool_) {
        if (item.joinable()) {
            item.join();
        }
    }
}

void ThreadPool::InitThreadPool(size_t thread_num)
{
    for (int i = 0; i < thread_num; ++i) {
        thread_pool_.emplace_back([this]() {
            cout << "thread [" << std::this_thread::get_id() << "] create..." << endl;
            FLUSH;
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(mtx_);
                    cond_.wait(lock, [this]() -> bool { return (stop_flag_ || !task_queue_.empty()); });
                    if (stop_flag_) {
                        cout << "thread [" << std::this_thread::get_id() << "] down..." << endl;
                        return;
                    }
                    task = std::move(task_queue_.front());
                    task_queue_.pop();
                }
                cout << "task start working in thread [" << std::this_thread::get_id() << "]" << endl;
                task();
                cout << "task finish working in thread [" << std::this_thread::get_id() << "]" << endl;
                FLUSH;
            }
        });
    }
}

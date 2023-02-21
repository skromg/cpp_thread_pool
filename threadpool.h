//
// Created by swh on 2/21/23.
//

#ifndef THREAD_POOL_THREADPOOL_H
#define THREAD_POOL_THREADPOOL_H

#include <cstddef>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <vector>
#include <queue>
#include <functional>

using std::cout;
using std::endl;
using std::flush;

#define FLUSH flush(cout)

class ThreadPool
{
public:
    explicit ThreadPool(size_t thread_num);
    ~ThreadPool();

    template <typename F, typename... Args>
    auto EnqueueTask(F &&f, Args &&...args) -> std::future<std::invoke_result_t<F, Args...>>;

private:
    std::vector<std::thread> thread_pool_;
    using Task = std::function<void()>;
    std::queue<Task> task_queue_;
    std::mutex mtx_;
    std::condition_variable cond_;
    bool stop_flag_{ false };
};

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

ThreadPool::ThreadPool(size_t thread_num)
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

template <typename F, typename... Args>
auto ThreadPool::EnqueueTask(F &&f, Args &&...args) -> std::future<std::invoke_result_t<F, Args...>>
{
    using return_type = std::invoke_result_t<F, Args...>;
    auto task =
        std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mtx_);
        task_queue_.template emplace([task]() { (*task)(); });
        cond_.notify_one();
    }
    return res;
}

#endif // THREAD_POOL_THREADPOOL_H

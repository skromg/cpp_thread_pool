#include <iostream>
#include <thread>
#include <QApplication>
#include "testwidget.h"
#include "threadpool.h"

using std::cout;
using std::endl;
using std::flush;

#define FLUSH flush(cout)

int func()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 9;
}

int main(int argc, char *argv[])
{
    cout << "main thread [" << std::this_thread::get_id() << "] start working..." << endl;
    ThreadPool thread_pool(5);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::future<int> task_future = thread_pool.EnqueueTask([]() {
        cout << "task start ... " << endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        cout << "task end ... " << endl;
        FLUSH;
        return 9;
    });
    cout << "start waiting res ..." << endl;
    int res = task_future.get();
    cout << "finish waiting res ..." << endl;
    cout << "the res is " << res << endl;
    FLUSH;

    return 0;
}

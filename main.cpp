#include <iostream>
#include <thread>
#include <QApplication>
#include <tuple>
#include <utility>
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
    std::this_thread::sleep_for(std::chrono::seconds(3));

    size_t num = 10;
    std::vector<std::future<std::tuple<int, int>>> results;
    for (int i = 0; i < num; ++i) {
        results.emplace_back(thread_pool.EnqueueTask([i]() {
            cout << "task " << i << " start ... " << endl;
            std::this_thread::sleep_for(std::chrono::seconds(10));
            cout << "task " << i<< " end ... " << endl;
            FLUSH;
            return std::make_tuple(i, i*i);
        }));
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    for(auto &item : results)
    {
        cout << "wait result ..." << endl;
        auto [index, result] = item.get();
        cout << "task " << index << ", result is " << result << endl;
    }

    return 0;
}

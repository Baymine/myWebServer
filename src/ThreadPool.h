//
// Created by root on 2023/1/12.
//
#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

//    void add(std::function<void()>);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;
};


template<class F, class... Args>
auto ThreadPool::add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
    // result_of: 动机是确定调用可调用 (Callable) 类型的结果，尤其是结果类型对不同参数集不同的情况
    using return_type = typename std::result_of<F(Args...)>::type;  // 确定可调用对象F的返回值类型

    // The class template std::packaged_task wraps any Callable target
    // (function, lambda expression, bind expression, or another function object)
    // so that it can be invoked asynchronously.
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex>lock(tasks_mtx);

        // don't allow enqueueing after stopping the pool
        if (stop){
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task](){ (*task)(); });
    }
    cv.notify_one();
    return res;
}

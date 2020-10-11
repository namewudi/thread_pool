#pragma once
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <iostream>
#include <thread>

class thread_pool_VT {
public:
    explicit thread_pool_VT(size_t thread_count)
        : data_(std::make_shared<data>()) {
        for (size_t i = 0; i < thread_count; ++i) {
            std::thread([data = data_] {
                std::unique_lock<std::mutex> lk(data->mtx_);
                for (;;) {
                    if (!data->tasks_.empty()) {
                        auto current = std::move(data->tasks_.front());
                        data->tasks_.pop();
                        lk.unlock();
                        current();
                        lk.lock();//阻塞在任务运行结束，逻辑判断之前
                    }
                    else if (data->is_shutdown_) {
                        break;
                    }
                    else {
                        data->cond_.wait(lk);
                    }
                }
                }).detach();
        }
    }

    thread_pool_VT() = default;
    thread_pool_VT(thread_pool_VT&&) = default;

    ~thread_pool_VT() {
        if ((bool)data_) {
            {
                std::lock_guard<std::mutex> lk(data_->mtx_);
                data_->is_shutdown_ = true;
            }
            data_->cond_.notify_all();
        }
    }

    template <class F, class... ARGS>
    void execute(F&& task, ARGS&&... args) {
        {
            std::lock_guard<std::mutex> lk(data_->mtx_);
            data_->tasks_.emplace(std::bind(std::forward<F>(task), std::forward<ARGS>(args)...));
        }
        data_->cond_.notify_one();
    }

private:
    struct data {
        std::mutex mtx_;
        std::condition_variable cond_;
        bool is_shutdown_ = false;
        std::queue<std::function<void()>> tasks_;
    };
    std::shared_ptr<data> data_;
};
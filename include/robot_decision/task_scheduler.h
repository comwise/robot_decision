#ifndef __COMWISE__ROBOT_DECISION__TASK_SCHEDULER__H__
#define __COMWISE__ROBOT_DECISION__TASK_SCHEDULER__H__

#include <memory>
#include <atomic>
#include <string>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "task.h"

class TaskScheduler {
public:
    TaskScheduler() = default;
    virtual ~TaskScheduler() {
        stop();
    }

    void submit(std::shared_ptr<Task> task) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        task_queue_.push(task);
        cv_.notify_one();
    }

    void start() {
        processor_thread_ = std::thread([this] {
            while (!stop_flag_) {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                cv_.wait(lock, [this] { 
                    return !task_queue_.empty() || stop_flag_; 
                });
                
                if (stop_flag_) {
                    break;
                }
                
                auto task = task_queue_.front();
                task_queue_.pop();
                lock.unlock();
                
                if (task) {
                    task->execute();
                }
            }
        });
    }

    void stop() {
        stop_flag_ = true;
        cv_.notify_all();
        if (processor_thread_.joinable()) { 
            processor_thread_.join();
        }
    }

private:
    std::queue<std::shared_ptr<Task>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_flag_{false};
    std::thread processor_thread_;
};

#endif // __COMWISE__ROBOT_DECISION__TASK_SCHEDULER__H__
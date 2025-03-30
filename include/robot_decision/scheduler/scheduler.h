#ifndef __COMWISE__ROBOT_DECISION__SCHEDULER__H__
#define __COMWISE__ROBOT_DECISION__SCHEDULER__H__

#include <memory>
#include <atomic>
#include <string>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>

template <typename Type>
class Scheduler {
public:
    virtual ~Scheduler() = default;

    virtual void submit(std::shared_ptr<Type> task)  = 0;

    virtual void start() = 0;
    virtual void stop() = 0;
};

template <typename Derived, typename TaskType, typename QueueType>
class SchedulerBase :  public Scheduler<TaskType> {
public:
    virtual ~SchedulerBase() { stop(); }

    virtual void submit(std::shared_ptr<TaskType> task) = 0;

    virtual void start() override {
        worker_thread_ = std::thread(&SchedulerBase::worker_loop, this);
    }

    virtual void stop() override {
        stop_flag_ = true;
        queue_cv_.notify_all();
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }

protected:
    void worker_loop() {
        while (!stop_flag_) {
            auto task = this->dequeue_task();
            if (task) {
                try {
                    static_cast<Derived*>(this)->pre_execute(task);
                    task->execute();
                    static_cast<Derived*>(this)->post_execute(task);
                } catch (const std::exception& e) {
                    //handle_task_error(task, e);
                }
            }
        }
    }

    std::shared_ptr<TaskType> dequeue_task() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        queue_cv_.wait(lock, [this] { 
            return stop_flag_ || static_cast<Derived*>(this)->has_available_task(); 
        });

        if (stop_flag_) return nullptr;

        return static_cast<Derived*>(this)->select_next_task();
    }

protected:
    QueueType task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::atomic<bool> stop_flag_{false};
    std::thread worker_thread_;
};

#endif // __COMWISE__ROBOT_DECISION__SCHEDULER__H__
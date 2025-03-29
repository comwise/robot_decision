#ifndef __COMWISE__ROBOT_DECISION__FIFO_SCHEDULER__H__
#define __COMWISE__ROBOT_DECISION__FIFO_SCHEDULER__H__

#include <memory>
#include <mutex>
#include "scheduler.h"
#include "scheduler/task.h"

// 基础调度器实现（FIFO）
class FifoScheduler : public SchedulerBase<FifoScheduler, Task, std::queue<std::shared_ptr<Task>>> {
public:
    virtual void submit(std::shared_ptr<Task> task) override {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        task_queue_.push(std::move(task));
        queue_cv_.notify_one();
    }

protected:
    friend class SchedulerBase<FifoScheduler, Task, std::queue<std::shared_ptr<Task>>>;

    bool has_available_task() const {
        return !task_queue_.empty();
    }

    std::shared_ptr<Task> select_next_task() {
        auto task = task_queue_.front();
        task_queue_.pop();
        return task;
    }

    void pre_execute(std::shared_ptr<Task>& task) {}
    void post_execute(std::shared_ptr<Task>& task) {}
};

#endif // __COMWISE__ROBOT_DECISION__FIFO_SCHEDULER__H__
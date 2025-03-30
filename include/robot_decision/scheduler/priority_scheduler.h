#ifndef __COMWISE__ROBOT_DECISION__PRIORITY_SCHEDULER__H__
#define __COMWISE__ROBOT_DECISION__PRIORITY_SCHEDULER__H__

#include <memory>
#include <mutex>
#include <unordered_set>
#include "scheduler.h"
#include "scheduler/task.h"
#include "dependency_graph.h"

// 显式定义的比较器类型（替代 Lambda）
struct PriorityComparator {
    bool operator()(const std::shared_ptr<PriorityTask>& a,
                    const std::shared_ptr<PriorityTask>& b) const {
        return a->priority < b->priority;
    }
};

class PriorityScheduler : public SchedulerBase<PriorityScheduler, PriorityTask, 
                            std::priority_queue<
                                std::shared_ptr<PriorityTask>,
                                std::vector<std::shared_ptr<PriorityTask>>,
                                PriorityComparator>> {
public:
    void submit(std::shared_ptr<PriorityTask> task) override {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        
        // 记录依赖关系
        for (auto &depend : task->dependencies) {
            dep_graph_.addDependency(task->task_id, depend);
        }
        
        // 插入优先队列
        task_queue_.push(std::move(task));
        queue_cv_.notify_one();
    }

protected:
    friend class SchedulerBase<PriorityScheduler, PriorityTask, 
             std::priority_queue<std::shared_ptr<PriorityTask>,
             std::vector<std::shared_ptr<PriorityTask>>,
             PriorityComparator>>;

    bool has_available_task() const {
        //return !task_queue_.empty() && dep_graph_.isReady(task_queue_.top()->task_id);
        return !task_queue_.empty();
    }

    std::shared_ptr<PriorityTask> select_next_task() {
        auto task = std::move(const_cast<std::shared_ptr<PriorityTask>&>(task_queue_.top()));
        task_queue_.pop();
        return task;
    }

    void pre_execute(std::shared_ptr<PriorityTask>& task) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        executing_tasks_.insert(task->task_id);
    }

    void post_execute(std::shared_ptr<PriorityTask>& task) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        //dep_graph_.markCompleted(task->task_id);
        executing_tasks_.erase(task->task_id);
    }

private:
    std::unordered_set<std::string> executing_tasks_;
    DependencyGraph dep_graph_;
};

#endif // __COMWISE__ROBOT_DECISION__PRIORITY_SCHEDULER__H__
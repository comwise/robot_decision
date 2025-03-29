#ifndef __COMWISE__ROBOT_DECISION__PRIORITY_TASK__H__
#define __COMWISE__ROBOT_DECISION__PRIORITY_TASK__H__

#include <memory>
#include <atomic>
#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include "task.h"
#include "dependency_graph.h"


class PriorityTask {
public:
    struct TaskNode {
        std::shared_ptr<Task> task;
        TaskPriority priority;
        std::vector<std::string> dependencies;
    };

    void submit(TaskNode task_node) {
        std::lock_guard<std::mutex> lock(mutex_);
        dependency_graph_.addDependency(task_node.task->id(), task_node.dependencies.front());
        priority_queue_.push(std::move(task_node));
        cv_.notify_one();
    }

    void start() {
        worker_ = std::thread([this] {
            while (!stop_flag_) {
                processTasks();
            }
        });
    }

    void stop() {
        stop_flag_ = true;
        cv_.notify_all();
        if (worker_.joinable()) worker_.join();
    }

private:
    void processTasks() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !priority_queue_.empty() || stop_flag_; });

        auto execution_order = dependency_graph_.topologicalSort();
        for (auto& task_id : execution_order) {
            // 实现任务执行逻辑
            // 根据优先级和依赖关系执行任务
        }
    }

    struct PriorityCompare {
        bool operator()(const TaskNode& a, const TaskNode& b) {
            return a.priority < b.priority;
        }
    };

    DependencyGraph dependency_graph_;
    std::priority_queue<TaskNode, std::vector<TaskNode>, PriorityCompare> priority_queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_flag_{false};
    std::thread worker_;
};

#endif // __COMWISE__ROBOT_DECISION__PRIORITY_TASK__H__
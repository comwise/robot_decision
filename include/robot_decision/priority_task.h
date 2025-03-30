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
        int unresolved_deps {0};
    };

    void submit(TaskNode task_node) {
        std::lock_guard<std::mutex> lock(mutex_);
        // 初始化未解决依赖数
        task_node.unresolved_deps = task_node.dependencies.size();
        const std::string task_id = task_node.task->id();
        all_tasks_[task_id] = task_node;

        for (auto &depend : task_node.dependencies) {
            dependency_graph_.addDependency(task_node.task->id(), depend);
            reverse_deps_[depend].push_back(task_id);
        }
        // 如果没有依赖，直接加入就绪队列
        if (task_node.unresolved_deps == 0) {
            ready_queue_.push(std::move(task_node));
        }
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
        if (worker_.joinable()) {
            worker_.join();
        }
    }

private:
    void processTasks() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { 
            return !ready_queue_.empty() || stop_flag_; 
        });

        if (stop_flag_) return;

#if 0
        auto execution_order = dependency_graph_.topologicalSort();
        for (auto& task_id : execution_order) {
            // 实现任务执行逻辑
            // 根据优先级和依赖关系执行任务
        }
#else
        // 取出最高优先级任务
        TaskNode task_node = ready_queue_.top();
        ready_queue_.pop();
        lock.unlock();
#endif

        // 执行任务
        task_node.task->execute();

        // 更新依赖关系
        handleCompletedTask(task_node.task->id());
    }

    void handleCompletedTask(const std::string& completed_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // 遍历所有依赖此任务的任务
        for (const auto& dependent_id : reverse_deps_[completed_id]) {
            auto& dependent = all_tasks_[dependent_id];
            if (--dependent.unresolved_deps == 0) {
                ready_queue_.push(dependent);
                cv_.notify_one();
            }
        }
        
        // 清理已完成任务
        all_tasks_.erase(completed_id);
        reverse_deps_.erase(completed_id);
    }

    struct PriorityCompare {
        bool operator()(const TaskNode& a, const TaskNode& b) {
            return a.priority < b.priority;
        }
    };

private:
    std::priority_queue<TaskNode, std::vector<TaskNode>, PriorityCompare> ready_queue_;
    std::unordered_map<std::string, TaskNode> all_tasks_;
    std::unordered_map<std::string, std::vector<std::string>> reverse_deps_;
    DependencyGraph dependency_graph_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_flag_{false};
    std::thread worker_;
};

#endif // __COMWISE__ROBOT_DECISION__PRIORITY_TASK__H__
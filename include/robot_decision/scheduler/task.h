#ifndef __COMWISE__ROBOT_DECISION__ITASK__H__
#define __COMWISE__ROBOT_DECISION__ITASK__H__

#include <string>
#include <vector>

// 基础任务类型
struct ITask {
    virtual ~ITask() = default;
    virtual void execute() = 0;
};

struct Task : public ITask {
    virtual ~Task() = default;
    virtual void execute() override { }
    int task_type;
    std::string task_id;
    std::string task_name;
};

// 优先级调度器实现（带优先级和依赖）
struct PriorityTask : Task {
    int priority{0};
    std::vector<std::string> dependencies;
};

#endif // __COMWISE__ROBOT_DECISION__ITASK__H__
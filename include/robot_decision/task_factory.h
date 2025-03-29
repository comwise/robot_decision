#ifndef __COMWISE__ROBOT_DECISION__TASK_FACTORY__H__
#define __COMWISE__ROBOT_DECISION__TASK_FACTORY__H__

#include <memory>
#include <unordered_map>
#include <exception>
#include <iostream>
#include "task.h"

class TaskFactory {
public:
    using Creator = std::function<std::shared_ptr<Task>()>;

    static TaskFactory& instance() {
        static TaskFactory instance;
        return instance;
    }

    void registerCreator(TaskType type, Creator creator) {
        creators_[type] = creator;
    }

    std::shared_ptr<Task> createTask(TaskType type) {
        auto it = creators_.find(type);
        if (it != creators_.end()) {
            return it->second();
        } else { 
            std::cout << "unregistered task type: " << static_cast<int>(type) << std::endl;
            return nullptr;
        }
    }

private:
    std::unordered_map<TaskType, Creator> creators_;
};

#endif // __COMWISE__ROBOT_DECISION__TASK_FACTORY__H__
#ifndef __COMWISE__ROBOT_DECISION__TASK__H__
#define __COMWISE__ROBOT_DECISION__TASK__H__

#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include "data.h"

class Task {
public:
    struct Metadata {
        TaskType type;
        std::string id;
        std::unordered_map<std::string, std::string> parameters;
        int priority{0}; // 0-99，数值越大优先级越高
    };

    Task(Metadata meta, std::function<void()> callback)
        : meta_(std::move(meta)), callback_(std::move(callback)) { }

    void execute() const {
        if (callback_) 
            callback_();
    }

    const Metadata& getMetadata() const { return meta_; }

    const std::string& id() const { return meta_.id; }

private:
    Metadata meta_;
    std::function<void()> callback_;
};

#endif // __COMWISE__ROBOT_DECISION__TASK__H__
#ifndef __COMWISE__ROBOT_DECISION__TASK_RPC__H__
#define __COMWISE__ROBOT_DECISION__TASK_RPC__H__

#include <memory>
#include <atomic>
#include <string>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "task.h"

class TaskRpc {
public:
    TaskRpc() = default;
    virtual ~TaskRpc() { }

private:

};

#endif // __COMWISE__ROBOT_DECISION__TASK_RPC__H__
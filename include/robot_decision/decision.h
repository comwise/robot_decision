// -------------------- Decision --------------------
#ifndef __COMWISE__ROBOT_DECISION__DECISION__H__
#define __COMWISE__ROBOT_DECISION__DECISION__H__

#include <memory>
#include <iostream>
#include "navigation.h"
#include "action.h"
#include "state_manager.h"
#include "task_scheduler.h"
#include "fault_handler.h"

class Decision {
public:
    Decision(
        std::shared_ptr<INavigation> nav,
        std::shared_ptr<IAction> action) 
        : nav_module_(std::move(nav)),
        action_module_(std::move(action)),
        state_manager_(std::make_unique<StateManager>()),
        task_scheduler_(std::make_unique<TaskScheduler>()),
        fault_handler_(std::make_unique<FaultHandler>()) {
        
        init();
    }

    ~Decision() {
        deinit();
    }

    void submitTask(std::shared_ptr<Task> task) {
        if (task_scheduler_) {
            task_scheduler_->submit(task);
        }
    }

    void handleEmergencyStop() {
        if (state_manager_) {
            state_manager_->setState(State::EMERGENCY_STOP);
        }
        if (nav_module_) {
            nav_module_->pause();
        }
        if (action_module_) {
            action_module_->exec("emergency_stop");
        }
    }

    void clearError() {
        if (state_manager_->getState() == State::ERROR ||
            state_manager_->getState() == State::EMERGENCY_STOP) {
            state_manager_->setState(State::IDLE);
            error_type_ = ErrorType::NONE;
        }
    }

private:
    void init() {
        // state
        if (state_manager_) {
            state_manager_->registerHandler(
                State::IDLE, [this] { handleIdleState(); });
            state_manager_->registerHandler(
                State::TASK_RUNNING, [this] { handleTaskState(); });
            // ...
        }

        // task scheduler
        if (task_scheduler_) {
            task_scheduler_->start();
        }
    }

    void deinit() {
        if (task_scheduler_) {
            task_scheduler_->stop();
        }
    }

    void handleIdleState() {
        // idle state
        std::cout << "handle idle state" << std::endl;
    }

    void handleTaskState() {
        // running state
         std::cout << "handle running state" << std::endl;
    }

    void handleErrorState() {
        // running state
         std::cout << "handle running state" << std::endl;
    }

private:
    std::shared_ptr<INavigation> nav_module_;
    std::shared_ptr<IAction> action_module_;
    std::unique_ptr<StateManager> state_manager_;
    std::unique_ptr<TaskScheduler> task_scheduler_;
    std::unique_ptr<FaultHandler> fault_handler_;

    ErrorType error_type_;
};

#endif // __COMWISE__ROBOT_DECISION__DECISION__H__

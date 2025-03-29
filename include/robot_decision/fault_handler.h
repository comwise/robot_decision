// -------------------- fault handler --------------------
#ifndef __COMWISE__ROBOT_DECISION__FAULT_HANDLER__H__
#define __COMWISE__ROBOT_DECISION__FAULT_HANDLER__H__

#include <chrono>
#include <functional>
#include <unordered_map>
#include <thread>
#include "data.h"

class FaultHandler {
public:
    using ErrorType = ErrorType;
    struct FaultPolicy {
        int max_retries;
        std::chrono::milliseconds retry_interval;
        std::function<void()> recovery_action;
    };

    void registerPolicy(ErrorType type, FaultPolicy policy) {
        policies_[type] = std::move(policy);
    }

    void unregisterPolicy(ErrorType type) {
        auto cit = policies_.find(type);
        if (cit != policies_.end()) {
            policies_.erase(cit);
        }
    }

    void handleFault(ErrorType type) {
        auto it = policies_.find(type);
        if (it != policies_.end()) {
            executeRecovery(it->second);
        }
    }

private:
    void executeRecovery(const FaultPolicy& policy) {
        for (int i = 0; i < policy.max_retries; ++i) {
            try {
                policy.recovery_action();
                return;
            } catch (...) {
                std::this_thread::sleep_for(policy.retry_interval);
            }
        }
        escalateToCritical();
    }

    void escalateToCritical() {
        // to handler critical 
    }

private:
    std::unordered_map<ErrorType, FaultPolicy> policies_;
};

#endif // __COMWISE__ROBOT_DECISION__FAULT_HANDLER__H__
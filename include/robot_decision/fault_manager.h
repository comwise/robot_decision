#ifndef __COMWISE__ROBOT_DECISION__FAULT_MANAGER__H__
#define __COMWISE__ROBOT_DECISION__FAULT_MANAGER__H__

#include <memory>
#include <functional>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "rerty_policy.h"

class FaultManager {
public:
    struct RecoveryPlan {
        std::unique_ptr<RetryPolicy> retry_policy;
        std::function<void()> fallback;
    };

    void registerRecovery(ErrorLevel level, RecoveryPlan plan) {
        std::lock_guard<std::mutex> lck(recovery_mtx_);
        recovery_plans_[level] = std::move(plan);
    }

    void unregisterRecovery(ErrorLevel level) {
        std::lock_guard<std::mutex> lck(recovery_mtx_);
        auto cit = recovery_plans_.find(level);
        if (cit != recovery_plans_.end()) {
            recovery_plans_.erase(cit);
        }
    }

    void handleError(ErrorLevel level, const std::function<void()>& handler) {
        auto it = recovery_plans_.find(level);
        if (it != recovery_plans_.end()) {
            executeWithRecovery(handler, *it->second.retry_policy, it->second.fallback);
        }
    }

private:
    void executeWithRecovery(const std::function<void()>& op, 
                            RetryPolicy& policy,
                            const std::function<void()>& fallback) {
        for (int attempt = 0;; ++attempt) {
            try {
                op();
                return;
            } catch (const std::exception& e) {
                if (!policy.shouldRetry(attempt, e)) {
                    fallback();
                    throw;
                }
                std::this_thread::sleep_for(policy.delay(attempt));
            }
        }
    }

private:
    std::unordered_map<ErrorLevel, RecoveryPlan> recovery_plans_;
    std::mutex recovery_mtx_;
};

#endif // __COMWISE__ROBOT_DECISION__FAULT_MANAGER__H__
#ifndef __COMWISE__ROBOT_DECISION__RETRY_POLICY__H__
#define __COMWISE__ROBOT_DECISION__RETRY_POLICY__H__

#include <memory>
#include <functional>
#include <chrono>
#include <exception>
#include "data.h"

class RetryPolicy {
public:
    virtual ~RetryPolicy() = default;

    virtual bool shouldRetry(int attempt, const std::exception& e) = 0;
    virtual std::chrono::milliseconds delay(int attempt) = 0;
};

class ExponentialBackoffPolicy : public RetryPolicy {
public:
    ExponentialBackoffPolicy(int max_retries, std::chrono::milliseconds initial_delay)
        : max_retries_(max_retries), initial_delay_(initial_delay) {}

    bool shouldRetry(int attempt, const std::exception&) override {
        return attempt < max_retries_;
    }

    std::chrono::milliseconds delay(int attempt) override {
        return initial_delay_ * (1 << attempt);
    }

private:
    int max_retries_{1};
    std::chrono::milliseconds initial_delay_;
};

#endif // __COMWISE__ROBOT_DECISION__RETRY_POLICY__H__
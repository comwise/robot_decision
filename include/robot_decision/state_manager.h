#ifndef __COMWISE__ROBOT_DECISION__STATE_MANAGER__H__
#define __COMWISE__ROBOT_DECISION__STATE_MANAGER__H__

#include <mutex>
#include <functional>
#include <unordered_map>
#include "data.h"

class StateManager {
public:
    using State = State;
    
    StateManager() : current_state_(State::IDLE) {}
    
    void setState(State new_state) {
        std::lock_guard<std::mutex> lock(mutex_);
        last_state_ = current_state_;
        current_state_ = new_state;
        notifyStateChange();
    }

    State getState() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return current_state_;
    }

    void registerHandler(State state, std::function<void()> handler) {
        state_handlers_[state] = std::move(handler);
    }

    void unregisterHandler(State state) {
        auto cit = state_handlers_.find(state);
        if (cit != state_handlers_.end()) {
            state_handlers_.erase(cit);
        }
    }

private:
    void notifyStateChange() {
        auto it = state_handlers_.find(current_state_);
        if (it != state_handlers_.end()) {
            it->second();
        }
    }

private:
    State current_state_;
    State last_state_;
    mutable std::mutex mutex_;
    std::unordered_map<State, std::function<void()>> state_handlers_;
};

#endif // __COMWISE__ROBOT_DECISION__STATE_MANAGER__H__
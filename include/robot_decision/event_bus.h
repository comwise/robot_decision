#ifndef __COMWISE__ROBOT_DECISION__EVENT_BUS__H__
#define __COMWISE__ROBOT_DECISION__EVENT_BUS__H__

#include <functional>
#include <mutex>
#include <unordered_map>

class EventBus {
public:
    using Handler = std::function<void(const std::string&)>;

    void subscribe(const std::string& topic, Handler handler) {
        std::lock_guard<std::mutex> lock(mutex_);
        subscribers_[topic].push_back(handler);
    }

    void publish(const std::string& topic, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (auto it = subscribers_.find(topic); it != subscribers_.end()) {
            for (auto& handler : it->second) {
                handler(message);
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<Handler>> subscribers_;
    std::mutex mutex_;
};

#endif // __COMWISE__ROBOT_DECISION__EVENT_BUS__H__
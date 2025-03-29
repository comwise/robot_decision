#include <memory>
#include <vector>
#include <iostream>
#include "action.h"
#include "navigation.h"
#include "decision.h"

int main() {

    auto nav = std::make_shared<Navigation>();
    auto action = std::make_shared<Action>();
    
    Decision core(nav, action);
    
    // 创建导航任务
    auto nav_task = std::make_shared<Task>(
        Task::Metadata{TaskType::NAVIGATION, "nav_001", {{"target", "room_101"}}, 0},
        [] { std::cout << "Navigation completed!" << std::endl; }
    );
    
    // 创建操作任务
    auto action_task = std::make_shared<Task>(
        Task::Metadata{TaskType::MANIPULATION, "manip_002", {{"action", "grab"}}, 0},
        [] { std::cout << "Action completed!" << std::endl; }
    );
    
    core.submitTask(nav_task);
    core.submitTask(action_task);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    core.handleEmergencyStop();
    
    return 0;
}

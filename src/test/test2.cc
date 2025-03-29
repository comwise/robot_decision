#include <iostream>
#include <memory>
#include <thread>
#include "event_bus.h"
#include "fault_manager.h"
#include "priority_task.h"
#include "task_factory.h"
#include "navigation.h"

int main() {

    // 初始化事件总线
    EventBus event_bus;

    // 注册任务工厂
    TaskFactory::instance().registerCreator(TaskType::NAVIGATION, [] {
        return std::make_shared<Task>(Task::Metadata(), nullptr);
    });

    // 配置错误恢复策略
    FaultManager fault_mgr;
#if 0
    fault_mgr.registerRecovery(ErrorLevel::RECOVERABLE, {
        std::make_unique<ExponentialBackoffPolicy>(3, std::chrono::milliseconds(100)),
        [] { /* 回退操作 */ }
    });
#endif

    // 创建增强调度器
    PriorityTask scheduler;
    scheduler.start();

    // 提交带依赖关系的任务
    scheduler.submit({
        .task = TaskFactory::instance().createTask(TaskType::NAVIGATION),
        .priority = TaskPriority::HIGH,
        .dependencies = {"task1", "task2"}
    });

    // 模块通信示例
    //Navigation nav_module(event_bus);
    //event_bus.publish("navigation", "room_101");

    std::this_thread::sleep_for(std::chrono::seconds(2));
    scheduler.stop();

    return 0;
}
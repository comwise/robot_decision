#include <iostream>
#include <memory>
#include <thread>
#include "scheduler/fifo_scheduler.h"
#include "scheduler/priority_scheduler.h"

int main() {

    FifoScheduler basic;
    auto task1 = std::make_shared<Task>();
    basic.submit(std::move(task1));
    basic.start();

    // 增强型调度器使用
    PriorityScheduler enhanced;
    auto task2 = std::make_shared<PriorityTask>();
    task2->task_id = "task2";
    task2->priority = 2;
    enhanced.submit(std::move(task2));
    enhanced.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    basic.stop();
    enhanced.stop();

    return 0;
}
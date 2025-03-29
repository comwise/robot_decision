#ifndef __COMWISE__ROBOT_DECISION__DECISION_DATA__H__
#define __COMWISE__ROBOT_DECISION__DECISION_DATA__H__

#include <memory>
#include <vector>
#include <cmath>

enum class State {
    IDLE, 
    NORMAL,
    TASK_RUNNING, 
    EMERGENCY_STOP,
    ERROR,
    AVOIDING
};

enum class TaskType { 
    NAVIGATION,
    MANIPULATION,
    COMPOSITE
};

enum class TaskPriority { 
    LOW         = 0,
    NORMAL      = 1,
    HIGH        = 2,
    CRITICAL    = 3
};

enum class ErrorType { 
    NONE,
    MOTOR_FAILURE,
    SENSOR_FAILURE,
    COLLISION 
};

enum class ErrorLevel { 
    WARNING,
    RECOVERABLE,
    CRITICAL
};

struct Pose { 
    double x;
    double y;
    union {
       double theta;
       double angle;
       double z;
    };
};

#endif // __COMWISE__ROBOT_DECISION__DECISION_DATA__H__

#ifndef __COMWISE__ROBOT_DECISION__IACTION__H__
#define __COMWISE__ROBOT_DECISION__IACTION__H__

#include <string>
#include <iostream>

class IAction {
public:
    virtual ~IAction() = default;

    virtual void exec(const std::string& action) = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void cancel() = 0;
};

class Action : public IAction {
public:
    void exec(const std::string& action) override {
        std::cout << "action exec: " << action << std::endl;
    }

    void pause() override {
        std::cout << "action paused" << std::endl;
    }

    void resume() override {
        std::cout << "action resumed" << std::endl;
    }

    void cancel() override {
        std::cout << "action canceled" << std::endl;
    }
};

#endif // __COMWISE__ROBOT_DECISION__IACTION__H__
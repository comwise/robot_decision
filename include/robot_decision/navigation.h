#ifndef __COMWISE__ROBOT_DECISION__INAVIGATION__H__
#define __COMWISE__ROBOT_DECISION__INAVIGATION__H__

#include <string>
#include <iostream>

class INavigation {
public:
    virtual ~INavigation() = default;

    virtual void navigateTo(const std::string& target) = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void cancel() = 0;
};

class Navigation : public INavigation {
public:
    void navigateTo(const std::string& target) override {
        std::cout << "Navigating to: " << target << std::endl;
    }

    void pause() override {
        std::cout << "Navigation paused" << std::endl;
    }

    void resume() override {
        std::cout << "Navigation resumed" << std::endl;
    }

    void cancel() override {
        std::cout << "Navigation canceled" << std::endl;
    }
};

#endif // __COMWISE__ROBOT_DECISION__INAVIGATION__H__
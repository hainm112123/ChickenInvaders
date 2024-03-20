#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include "../common/common.h"

class Timer {
    double duration = 0, currentTime = 0;
    bool active = false;
public:
    Timer(double duration = 0);

    double RemainTime() const {
        return max(duration - currentTime, double(0));
    }

    void setDuration(double _duration);
    void startCountdown();
    bool timeIsUp();
    void deactive();
    void process();
};

class TimeManager {
    uint64_t currentTicks, lastFrameTicks, elapsedTicks;
    double elapsedTime;
    double FPS;
    static TimeManager *instance;
public:
    TimeManager();
    ~TimeManager();

    static TimeManager *Instance() {
        if (instance == nullptr) instance = new TimeManager();
        return instance;
    }
    double getFPS() const {
        return FPS;
    }
    double getElapsedTime() const {
        return elapsedTime;
    }

    void process();

};

#endif // TIME_H_INCLUDED

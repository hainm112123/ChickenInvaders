#include "time.h"

//.........................timer........................................
Timer::Timer(double _duration): duration(_duration) {}
void Timer::setDuration(double _duration) {
    duration = _duration;
}

void Timer::startCountdown() {
    active = true;
    st = CLOCK_NOW();
}

bool Timer::timeIsUp() {
    if (!active) return true;
    ed = CLOCK_NOW();
    ElapsedTime elapsed = ed - st;
    if (elapsed.count() > duration) {
        active = false;
        return true;
    }
    return false;
}

void Timer::deactive() {
    active = false;
}

//.......................TimeManager..............................................
TimeManager::TimeManager() {}

void TimeManager::process() {

}

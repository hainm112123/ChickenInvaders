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
TimeManager *TimeManager::instance = nullptr;
TimeManager::TimeManager(): currentTicks(SDL_GetPerformanceCounter()), lastFrameTicks(SDL_GetPerformanceCounter()), elapsedTicks(SDL_GetPerformanceCounter()), elapsedTime(0), FPS(0) {}
TimeManager::~TimeManager() {
    instance = nullptr;
}

void TimeManager::process() {
    currentTicks = SDL_GetPerformanceCounter();
    elapsedTicks = currentTicks - lastFrameTicks;
    lastFrameTicks = currentTicks;
    elapsedTime = elapsedTicks / double(SDL_GetPerformanceFrequency());
    FPS = (elapsedTime != 0) ? double(1) / elapsedTime : 0;
}

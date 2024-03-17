#include "time.h"

//.........................timer........................................
Timer::Timer(double _duration): duration(_duration) {}
void Timer::setDuration(double _duration) {
    duration = _duration;
}

void Timer::startCountdown() {
    active = true;
    currentTime = 0;
}

bool Timer::timeIsUp() {
    if (!active) return true;
    if (currentTime >= duration) {
        active = false;
        return true;
    }
    return false;
}

void Timer::process() {
    if (!active) return;
    currentTime += TimeManager::Instance()->getElapsedTime();
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

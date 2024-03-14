#include "fried_chicken.h"

FriedChicken::FriedChicken(double _x, double _y) {
    type = FRIED_CHICKEN;
    rect = {int(_x), int(_y), FRIED_CHICKEN_WIDTH, FRIED_CHICKEN_HEIGHT};
    setPosition(_x, _y);
    dir = (Rand(0, 100) < 50) ? 1 : 1;
    setStep(Rand(FRIED_CHICKEN_STEP_X_MIN, FRIED_CHICKEN_STEP_X_MAX) * dir, Rand(FRIED_CHICKEN_STEP_Y_MIN, FRIED_CHICKEN_STEP_Y_MAX));
    turn_timer = FRIED_CHICKEN_TURN_DELAY;
    active = true;
}

bool FriedChicken::handleMove() {
    if (!active) return false;

    turn_timer -= TimeManager::Instance()->getElapsedTime();
    if (turn_timer < 0) {
        updateStep(-15 * dir, 20);
//        cout << step_x << " " << step_y << "\n";
    }
    _move();

    if (x < 0 || x > double(SCREEN_WIDTH) - rect.w) {
        dir *= -1;
        step_x *= -1;
//        cout << "Touched Side\n";
    }

    if (y > double(SCREEN_HEIGHT) + rect.h + 50) {
        active = false;
    }

    while (turn_timer < 0) {
        turn_timer += FRIED_CHICKEN_TURN_DELAY;
    }
    return true;
}

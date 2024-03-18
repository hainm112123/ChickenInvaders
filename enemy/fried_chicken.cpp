#include "fried_chicken.h"

FriedChicken::FriedChicken(double _x, double _y) {
    type = FRIED_CHICKEN;
    rect = {int(_x), int(_y), FRIED_CHICKEN_WIDTH, FRIED_CHICKEN_HEIGHT};
    setPosition(_x, _y);
    dir = (Rand(0, 100) < 50) ? 1 : -1;
    setStep(Rand(FRIED_CHICKEN_STEP_X_MIN, FRIED_CHICKEN_STEP_X_MAX) * dir, 0);
    turn_timer = FRIED_CHICKEN_TURN_DELAY;
    delay = DELAY_AFTER_TOUCH_BOTTOM;
    active = true;
}

FriedChicken::~FriedChicken() {}

bool FriedChicken::handleMove() {
    if (!active) return false;

    turn_timer -= TimeManager::Instance()->getElapsedTime();
    if (turn_timer < 0) {
        updateStep(-1 * dir, 20);
//        cout << step_x << " " << step_y << "\n";
    }
    if (touch_bottom <= 3) _move();

    if (x < 0 || x > double(SCREEN_WIDTH) - rect.w) {
        dir *= -1;
        step_x *= -1;
//        cout << "Touched Side\n";
    }

    if (y > (double)(SCREEN_HEIGHT) - rect.h) {
        if (touch_bottom <= 3) {
            touch_bottom ++;
            step_y = -step_y + min(step_y - 150, (double)300);
//            cout << "Touched bottom " << step_y << "\n";
        }
        else {
            delay -= TimeManager::Instance()->getElapsedTime();
            if (delay < 0) active = false;
        }
    }

    while (turn_timer < 0) {
        turn_timer += FRIED_CHICKEN_TURN_DELAY;
    }
    return true;
}

void FriedChicken::render_friedchicken(SDL_Renderer *renderer) {
    int n = textures.size();
    if (touch_bottom <= 3) currentTime += TimeManager::Instance()->getElapsedTime();
    double per_pic = SECOND_PER_PICTURE_FASTER;
    if (currentTime >= per_pic * n) currentTime -= per_pic * n;
    SDL_RenderCopy(renderer, textures[int(currentTime / per_pic)].texture, NULL, &rect);
}

#ifndef FRY_CHICKEN_H_INCLUDED
#define FRY_CHICKEN_H_INCLUDED

#include "../common/entity.h"
#include "../common/time.h"

const int FRIED_CHICKEN_WIDTH = 36;
const int FRIED_CHICKEN_HEIGHT = 28;

const double FRIED_CHICKEN_TURN_DELAY = 0.04;
const int NUMBER_OF_FRIED_CHICKEN[] = {1, 15};

const int FRIED_CHICKEN_STEP_X_MIN = 360;
const int FRIED_CHICKEN_STEP_X_MAX = 500;
const int FRIED_CHICKEN_STEP_Y_MIN = 200;
const int FRIED_CHICKEN_STEP_Y_MAX = 300;

class FriedChicken: public Entity {
    double turn_timer;
    int dir;
    bool active;
public:
    FriedChicken(double _x, double _y);
    ~FriedChicken();
    bool handleMove();
};

#endif // FRY_CHICKEN_H_INCLUDED

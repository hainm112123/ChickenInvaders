#ifndef ROCKET_H_INCLUDED
#define ROCKET_H_INCLUDED

#include "../common/entity.h"
#include "../common/time.h"

const int ROCKET_WIDTH = 35;
const int ROCKET_HEIGHT = 95;
const int BIG_EXPLOSION_WIDTH = 429;
const int BIG_EXPLOSION_HEIGHT = 453;
const double MOVE_TIME = 3;

class Rocket: public Entity {
    int damage;
    double src_x, src_y, dst_x, dst_y;
    double R, distance, center_x, center_y;
public:
    Rocket(double _src_x, double _src_y, double _dst_x, double _dst_y);
    ~Rocket();

    bool reached();
    void handleMove();
    void reset();
};

#endif // ROCKET_H_INCLUDED

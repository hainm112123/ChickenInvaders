#ifndef ROCK_H_INCLUDED
#define ROCK_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

const double ROCK_HP = 300;
const double ROCK_HP_UPGRADE = 200;

const int MIN_ROCK_FALL_SPEED = 490;
const int MAX_ROCK_FALL_SPEED = 810;

const int MIN_ROCK_SIDE_SPEED_X = 200;
const int MAX_ROCK_SIDE_SPEED_X = 360;
const int MIN_ROCK_SIDE_SPEED_Y = 81;
const int MAX_ROCK_SIDE_SPEED_Y = 256;

const int NG_ROCK_SPEED = 200;

const int MIN_ROCK_SIZE = 36;
const int MAX_ROCK_SIZE = 81;

class Rock : public Entity{
    double hp;
    bool active = false;

public:
    using Entity::Entity;
    bool isActive() {
        return active;
    }
    void setActive(bool _active);    void setHP(double _hp);

    bool receiveDamage(double dmg);
    void handleMove();
};

#endif // ROCK_H_INCLUDED

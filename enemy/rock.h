#ifndef ROCK_H_INCLUDED
#define ROCK_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

const double ROCK_HP = 300;
const double ROCK_HP_UPGRADE = 200;

const int MIN_ROCK_FALL_SPEED = 360;
const int MAX_ROCK_FALL_SPEED = 720;

const int MIN_ROCK_SIDE_SPEED_X = 169;
const int MAX_ROCK_SIDE_SPEED_X = 333;
const int MIN_ROCK_SIDE_SPEED_Y = 64;
const int MAX_ROCK_SIDE_SPEED_Y = 256;

const int NG_ROCK_SPEED = 64;

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

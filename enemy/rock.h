#ifndef ROCK_H_INCLUDED
#define ROCK_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

const int ROCK_HP = 320;
const int ROCK_HP_UPGRADE = 200;

const int MIN_ROCK_FALL_SPEED = 400;
const int MAX_ROCK_FALL_SPEED = 600;

const int MIN_ROCK_SIDE_SPEED_X = 300;
const int MAX_ROCK_SIDE_SPEED_X = 500;
const int MIN_ROCK_SIDE_SPEED_Y = 100;
const int MAX_ROCK_SIDE_SPEED_Y = 300;

const int NG_ROCK_SPEED = 200;

const int MIN_ROCK_SIZE = 32;
const int MAX_ROCK_SIZE = 48;

class Rock : public Entity{
    int hp;
    bool active = false;

public:
    using Entity::Entity;
    bool isActive() {
        return active;
    }
    void setActive(bool _active);    void setHP(int _hp);

    void receiveDamage(int dmg);
    void handleMove();
};

#endif // ROCK_H_INCLUDED

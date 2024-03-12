#ifndef ROCK_H_INCLUDED
#define ROCK_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

const int ROCK_HP = 120;
const int ROCK_HP_UPGRADE = 80;

const int MIN_ROCK_FALL_SPEED = 4;
const int MAX_ROCK_FALL_SPEED = 6;

const int MIN_ROCK_SIDE_SPEED_X = 3;
const int MAX_ROCK_SIDE_SPEED_X = 5;
const int MIN_ROCK_SIDE_SPEED_Y = 1;
const int MAX_ROCK_SIDE_SPEED_Y = 3;

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

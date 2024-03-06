#ifndef ROCK_H_INCLUDED
#define ROCK_H_INCLUDED

#include "../init.h"

const int ROCK_HP = 30;

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
    void setActive(bool _active);
    bool isActive() {
        return active;
    }

    void receiveDamage(int dmg);
    void handleMove();
};

#endif // ROCK_H_INCLUDED

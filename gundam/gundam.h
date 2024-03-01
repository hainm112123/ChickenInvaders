#ifndef GUNDAM_H_INCLUDED
#define GUNDAM_H_INCLUDED

#include<set>

#include "../weapon/bullet.h"
#include "../weapon/weapon.h"
#include "../init.h"

const int GUNDAM_SIZE = 20;
const int GUNDAM_SPEED = 4;

enum GundamMove {
    GUNDAM_MOVE_UP = 0,
    GUNDAM_MOVE_LEFT,
    GUNDAM_MOVE_DOWN,
    GUNDAM_MOVE_RIGHT,
};
const int gundam_step_x[] = {0, -1, 0, 1};
const int gundam_step_y[] = {-1, 0, 1, 0};

const SDL_Keycode MoveKeyCode[] = {
    SDLK_w,
    SDLK_a,
    SDLK_s,
    SDLK_d,
};

class Gundam {
    Entity entity;
    set<Bullet*>bullets;
    Weapon weapon;
    int bulletDmg;
    int lives;
    bool alive;

public:
    Gundam();

    set<Bullet*> getBullets() const {
        return bullets;
    }
    Entity* getEntity() {
        return &entity;
    }
    int getBulletDamage() const {
        return bulletDmg;
    }
    bool isAlive() const {
        return alive;
    }

    void render(SDL_Renderer *renderer, Painter *painter);

    void _move();
    void control(SDL_Event event);
    void handleBullet(SDL_Renderer *renderer);
    void removeBullet(Bullet *bullet);
    void dead();
    bool revive();
};

#endif // GUNDAM_H_INCLUDED

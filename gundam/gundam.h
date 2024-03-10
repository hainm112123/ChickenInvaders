#ifndef GUNDAM_H_INCLUDED
#define GUNDAM_H_INCLUDED

#include<set>

#include "../weapon/bullet.h"
#include "../init.h"

const int GUNDAM_WIDTH = 65;
const int GUNDAM_HEIGHT = 67;
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

class Game;

class Gundam {
    Game *game;
    Entity entity;
    set<Bullet*>bullets;
    int lives;
    bool alive;

    vector<WeaponType> weapons;
    int currentWeaponID;
    int level;
    Entity shield;
    Entity laser;
    bool laserOn = false;

public:
    Gundam(Gallery *gallery);

    set<Bullet*> getBullets() const {
        return bullets;
    }
    Entity* getEntity() {
        return &entity;
    }
    int getBulletDamage();
    bool isAlive() const {
        return alive;
    }
    WeaponType getCurrentWeapon() const {
        return weapons[currentWeaponID];
    }
    int getLevel() const {
        return level;
    }
    bool isLaserOn() const {
        return laserOn;
    }
    Entity getLaser() const {
        return laser;
    }
    int getLives() const {
        return lives;
    }

    void setGame(Game *_game);
    void setLaserOn(bool _laserOn);
    void addWeapon(WeaponType newWeapon);
    void changeWeapon();
    void levelUp();

    void render(SDL_Renderer *renderer, Gallery *gallery, bool hasShield = false, bool hasLaser = false);

    void _move();
    void control(SDL_Event event, Timer &gundamLaserTimer);
    void handleBullet(SDL_Renderer *renderer);
    void removeBullet(Bullet *bullet);
    void dead();
    bool revive();
};

#endif // GUNDAM_H_INCLUDED

#ifndef GUNDAM_H_INCLUDED
#define GUNDAM_H_INCLUDED

#include<set>
#include<vector>

#include "../weapon/bullet.h"
#include "../common/common.h"
#include "../common/entity.h"
#include "../game/media.h"
#include "../common/time.h"

const int GUNDAM_WIDTH = 65;
const int GUNDAM_HEIGHT = 67;
const double GUNDAM_SPEED = 500;

const int MAX_GUNDAM_LEVEL = 10;
const int NUMBER_OF_BULLET_FORM = 4;
const double FIRE_DELAY[] = {0.1, 0, 0.2, 0.81};

enum GundamMove {
    GUNDAM_MOVE_UP = 0,
    GUNDAM_MOVE_LEFT,
    GUNDAM_MOVE_DOWN,
    GUNDAM_MOVE_RIGHT,
    GUNDAM_FIRE,
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
    int level, bullet_form;
    Entity shield;
    Entity laser;
    bool laserOn = false;
    bool keydown[5];

    Timer fire_timer;

public:
    Gundam();
    ~Gundam();

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

    void render(SDL_Renderer *renderer, bool hasShield = false, bool hasLaser = false);

    void _move();
    void control(SDL_Event event, Timer &gundamLaserTimer);
    void handleBullet(SDL_Renderer *renderer, const vector<pair<double, double>> &enemy_positions);
    void removeBullet(Bullet *bullet);
    void dead();
    bool revive();

    void reset();
    void resetControl();
};

#endif // GUNDAM_H_INCLUDED

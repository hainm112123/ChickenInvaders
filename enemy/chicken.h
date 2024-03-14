#ifndef CHICKEN_H_INCLUDED
#define CHICKEN_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"
#include "../weapon/bullet.h"
#include "../game/media.h"

const int CHICKEN_WIDTH[] = {78, 200};
const int CHICKEN_HEIGHT[] = {65, 200};

const double CHICKEN_SPEED[] = {100, 300};
const double NG_CHICKEN_SPEED = 60;

const double CHICKEN_HP[] = {100, 4900};
const double CHICKEN_HP_UPGRADE[] = {69, 2500};

const int CHICKENS_DISTANCE[] = {30, 100};

const int CHICKEN_EGG_WIDTH[] = {15, 80};
const int CHICKEN_EGG_HEIGHT[] = {15, 80};
const int MIN_CHICKEN_EGG_SPEED[] = {150, 200};
const int MAX_CHICKEN_EGG_SPEED[] = {400, 490};
const double NG_CHICKEN_EGG_SPEED = 160;

const int CHICKEN_SCORE[] = {10, 200};
const int NG_CHICKEN_SCORE[] = {6, 121};

const double BULLET_DELAY = 0.3;

struct ChickenMoveState {
    bool goLeft, goRight, goUp, goDown;
};

class Chicken {
    Entity entity;
    double hp;
    set<Bullet*> bullets;
    int level;
    double speed, bulletSpeed;
    int bulletWidth, bulletHeight;
    Time lastBullet = CLOCK_NOW();
    ChickenMoveState moveState;

public:
    Chicken(int offsetX = 0, int offsetY = 0, int _level = 0, int game_difficulty = 0);

    Entity* getEntity() {
        return &entity;
    }

    bool operator < (const Chicken &other) const {
        return entity < other.entity;
    }

    int getNumberOfBullet() {
        return bullets.size();
    }
    set<Bullet*> getBullets() {
        return bullets;
    }
    int getLevel() const {
        return level;
    }
    double getSpeed() const {
        return speed;
    }
    Time getLastBullet() const {
        return lastBullet;
    }
    bool isAlive() {
        return hp > 0;
    }
    double getHP() const {
        return hp;
    }
    ChickenMoveState getMoveState() const {
        return moveState;
    }

    void setLastBullet(Time _lastBullet);

    bool receiveDamage(double dmg);
    void setMoveState(ChickenMoveState _moveState);
    void _move();
    void render(SDL_Renderer *renderer);
    void addBullet(Bullet *_bullet, Gallery *gallery);
    void removeBullet(Bullet *bullet);
    void handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets);
};

#endif // CHICKEN_H_INCLUDED

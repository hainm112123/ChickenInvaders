#ifndef CHICKEN_H_INCLUDED
#define CHICKEN_H_INCLUDED

#include "../init.h"
#include "../weapon/bullet.h"

const int CHICKEN_WIDTH[] = {80, 200};
const int CHICKEN_HEIGHT[] = {60, 200};

const int CHICKEN_SPEED[] = {1, 3};

const int CHICKEN_HP[] = {100, 1600};
const int CHICKEN_HP_UPGRADE[] = {50, 360};

const int CHICKENS_DISTANCE[] = {30, 100};

const int CHICKEN_EGG_WIDTH[] = {15, 80};
const int CHICKEN_EGG_HEIGHT[] = {15, 80};
const int MIN_CHICKEN_EGG_SPEED[] = {1, 2};
const int MAX_CHICKEN_EGG_SPEED[] = {3, 3};

const int CHICKEN_SCORE[] = {10, 200};
const int NG_CHICKEN_SCORE[] = {6, 121};

const double BULLET_DELAY = 0.5;

class Chicken {
    Entity entity;
    int hp;
    set<Bullet*> bullets;
    int level;
    int speed, bulletSpeed;
    int bulletWidth, bulletHeight;
    Time lastBullet = CLOCK_NOW();

public:
    Chicken(int offsetX = 0, int offsetY = 0, int _level = 0, int NG = 0);

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
    int getSpeed() const {
        return speed;
    }
    Time getLastBullet() const {
        return lastBullet;
    }
    bool isAlive() {
        return hp > 0;
    }

    void setLastBullet(Time _lastBullet);

    bool receiveDamage(int dmg);
    void _move(int step_x, int step_y);
    void render(SDL_Renderer *renderer);
    void addBullet(Bullet *_bullet, Gallery *gallery);
    void removeBullet(Bullet *bullet);
    void handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets);
};

#endif // CHICKEN_H_INCLUDED

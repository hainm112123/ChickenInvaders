#ifndef CHICKEN_H_INCLUDED
#define CHICKEN_H_INCLUDED

#include "../init.h"
#include "../weapon/bullet.h"

const int CHICKEN_WIDTH[] = {40, 120};
const int CHICKEN_HEIGHT[] = {40, 120};

const int CHICKEN_SPEED[] = {1, 3};

const int CHICKEN_HP[] = {10, 50};

const int CHICKENS_DISTANCE[] = {30, 100};

const int CHICKEN_EGG_WIDTH[] = {15, 80};
const int CHICKEN_EGG_HEIGHT[] = {15, 80};
const int CHICKEN_EGG_SPEED[] = {2, 2};

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
    Chicken(int offsetX = 0, int offsetY = 0, int _level = 0);

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

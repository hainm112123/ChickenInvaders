#ifndef CHICKEN_H_INCLUDED
#define CHICKEN_H_INCLUDED

#include <set>

#include "../init.h"
#include "../weapon/bullet.h"

const int CHICKEN_WIDTH[] = {40, 120};
const int CHICKEN_HEIGHT[] = {40, 120};

const int CHICKEN_SPEED[] = {1, 3};

const int CHICKEN_HP[] = {10, 50};

const int CHICKENS_DISTANCE[] = {5, 100};

const int CHICKEN_EGG_WIDTH[] = {15, 40};
const int CHICKEN_EGG_HEIGHT[] = {15, 40};
const int CHICKEN_EGG_SPEED[] = {2, 2};

class Chicken {
    Entity entity;
    int hp;
    set<Bullet*> bullets;
    EnemyMoveState moveState;
    int level;
    int speed, bulletSpeed;
    int bulletWidth, bulletHeight;

public:
    Chicken(int offsetX = 0, int offsetY = 0, int _level = 0);

    Entity* getEntity() {
        return &entity;
    }
    int getNumberOfBullet() {
        return bullets.size();
    }
    set<Bullet*> getBullets() {
        return bullets;
    }

    bool receiveDamage(int dmg);
    void _move();
    void render(SDL_Renderer *renderer);
    void addBullet(Bullet *_bullet);
    void removeBullet(Bullet *bullet);
    void handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets);
};

#endif // CHICKEN_H_INCLUDED

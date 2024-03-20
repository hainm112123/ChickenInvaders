#ifndef CHICKEN_H_INCLUDED
#define CHICKEN_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"
#include "../weapon/bullet.h"
#include "../game/media.h"

const int CHICKEN_WIDTH[] = {78, 200, 129};
const int CHICKEN_HEIGHT[] = {65, 200, 108};

const double CHICKEN_SPEED[] = {100, 300, 256};
const double NG_CHICKEN_SPEED = 60;

const double CHICKEN_HP[] = {100, 6789, 2500};
const double CHICKEN_HP_UPGRADE[] = {69, 3333, 1111};

const int CHICKENS_DISTANCE[] = {30, 100, 0};

const int CHICKEN_EGG_WIDTH[] = {15, 80, 0};
const int CHICKEN_EGG_HEIGHT[] = {15, 80, 0};
const int MIN_CHICKEN_EGG_SPEED[] = {150, 200, 0};
const int MAX_CHICKEN_EGG_SPEED[] = {400, 490, 0};
const double NG_CHICKEN_EGG_SPEED = 160;

const int CHICKEN_SCORE[] = {10, 200, 169};
const int NG_CHICKEN_SCORE[] = {6, 121, 81};

const double BULLET_DELAY = 0.3;

enum ChickenType {
    CHICKEN_SMALL,
    CHICKEN_BOSS,
    CHICKEN_DODGE,
};

struct ChickenMoveState {
    bool goLeft, goRight, goUp, goDown;
};

class Chicken {
    Entity entity;
    double hp;
    set<Bullet*> bullets;
    ChickenType type;
    double speed, bulletSpeed;
    int bulletWidth, bulletHeight;
    ChickenMoveState moveState;

public:
    Chicken(int offsetX = 0, int offsetY = 0, ChickenType _type = CHICKEN_SMALL, int game_difficulty = 0);
    ~Chicken();

    Timer bulletTimer;

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
    ChickenType chicken_type() const {
        return type;
    }
    double getSpeed() const {
        return speed;
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

    bool receiveDamage(double dmg);
    void setMoveState(ChickenMoveState _moveState);
    void _move();
    void render(SDL_Renderer *renderer);
    void addBullet(Bullet *_bullet);
    void removeBullet(Bullet *bullet);
    void handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets);
};

#endif // CHICKEN_H_INCLUDED

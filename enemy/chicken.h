#ifndef CHICKEN_H_INCLUDED
#define CHICKEN_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"
#include "../weapon/bullet.h"
#include "../game/media.h"
#include "../gundam/gundam.h"

const int CHICKEN_WIDTH[] = {78, 200, 129};
const int CHICKEN_HEIGHT[] = {65, 200, 108};
const int CHICKEN_TELEPORT_WIDTH = 169;
const int CHICKEN_TELEPORT_HEIGHT = 196;
const int CHICKEN_ROCKET_WIDTH = 69;
const int CHICKEN_ROCKET_HEIGHT = 190;
const int BOSS_LASER_WIDTH = 433;
const int BOSS_LASER_HEIGHT = 720;

const double CHICKEN_SPEED[] = {96, 300, 256};
const double NG_CHICKEN_SPEED = 36;
const int CHICKEN_CIRCULAR_DISTANCE_MAX = 600;
const int CHICKEN_CIRCULAR_DISTANCE_MIN = 60;
const double CIRCULAR_CENTER_X = SCREEN_WIDTH / 2;
const double CIRCULAR_CENTER_Y = SCREEN_HEIGHT / 2;

const double CHICKEN_HP[] = {100, 5678, 2500};
const double CHICKEN_HP_UPGRADE[] = {69, 3210, 1111};

const int CHICKENS_DISTANCE[] = {30, 100, 0};

const int CHICKEN_EGG_WIDTH[] = {15, 56, 0};
const int CHICKEN_EGG_HEIGHT[] = {15, 52, 0};
const int MIN_CHICKEN_EGG_SPEED[] = {150, 200, 0};
const int MAX_CHICKEN_EGG_SPEED[] = {400, 444, 0};
const double NG_CHICKEN_EGG_SPEED = 160;
const double ROCKET_SPEED = 400;

const int CHICKEN_BOMB_EXPLOSION_WIDTH = 169;
const int CHICKEN_BOMB_EXPLOSION_HEIGHT = 169;
const int CHICKEN_ROCKET_EXPLOSION_WIDTH = 256;
const int CHICKEN_ROCKET_EXPLOSION_HEIGHT = 256;

const int CHICKEN_SCORE[] = {10, 200, 169};
const int NG_CHICKEN_SCORE[] = {6, 121, 81};

const double BULLET_DELAY = 0.3;
const double CHICKEN_TELEPORT_COOLDOWN = 10;
const double CHICKEN_TELEPORT_DURATION = NUMBER_OF_TELEPORT_PIC * SECOND_PER_PICTURE_FASTER * 2;
const double CHICKEN_ROCKET_INIT = 3;
const double CHICKEN_ROCKET_COOLDOWN = 15;
const double CHICKEN_ROCKET_DELAY = 1.5;
const double CHICKEN_ROCKET_MOVE_TIME = 0.4321;
const double CHICKEN_LASER_COOLDOWN = 20;
const double CHICKEN_SKILL_COOLDOWN = 5;

enum ChickenType {
    CHICKEN_SMALL,
    CHICKEN_BOSS,
    CHICKEN_DODGE,
};
enum ChickenMoveType {
    CHICKEN_BASIC_MOVE = 0,
    CHICKEN_CIRCULAR_MOVE
};

struct ChickenMoveState {
    bool goLeft, goRight, goUp, goDown;
};

class Chicken {
    Entity entity;

    Game *game;

    double hp;
    set<Bullet*> bullets;
    ChickenType type;
    double speed, bulletSpeed;
    int bulletWidth, bulletHeight;
    ChickenMoveState moveState;
    ChickenMoveType moveType;
    double circular_distance, angle;
    int direction = 1;
    bool onTeleport = false, usingLaser = false;
    bool onRocket = false, rocketExploding = false;
    double rocketTimeCounter = 0, laserTimeCounter = 0;
    int num_laser_left = 10;

    deque<Entity*> explosions, sparks;

public:
    Chicken(Game *_game, ChickenType _type = CHICKEN_SMALL, ChickenMoveType _moveType = CHICKEN_BASIC_MOVE, int game_difficulty = 0, vector<int>args = {});
    ~Chicken();

    Entity teleport, big_explosion, rocket, laser;
    Timer bulletTimer;
    Timer teleportCooldown, teleportDuration;
    Timer rocketInit, rocketCooldown;
    Timer laserCooldown, skillCooldown;

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
    bool OnTeleport() const {
        return onTeleport;
    }
    bool OnRocket() const {
        return onRocket;
    }
    bool UsingLaser() const {
        return usingLaser;
    }

    void setOnTeleport(bool val);
    void setOnRocket(bool val);
    bool receiveDamage(double dmg);
    void setMoveState(ChickenMoveState _moveState);
    void _move();
    void render(SDL_Renderer *renderer);
    void addBullet(Bullet *_bullet);
    void removeBullet(Bullet *bullet, vector<Bullet*> &gameEnemyBullets);
    void handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets);
    void handleExplosion(SDL_Renderer *renderer);
    bool checkBulletCollision(Entity *other);
    void timerProcess();
    void handleRocket(SDL_Renderer *renderer, double _dest_x, double _dest_y);
    void useRocket();
    void useLaser();
    void handleLaser(SDL_Renderer *renderer, int gundam_x);
};

#endif // CHICKEN_H_INCLUDED

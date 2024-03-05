#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../init.h"
#include "../gundam/gundam.h"
#include "../Painter.h"
#include "../enemy/chicken.h"
#include "../gundam/upgrade.h"

const int NUMBER_OF_CHICKEN = 30;
const int NUMBER_OF_CHICKEN_PER_ROW = 10;
const int MINI_BOSS_ROUND = 3;
const int BOSS_ROUND = 4;
const int UPGRADE_SPEED = 1;
const int SCREEN_SPEED = 2;
const int BG_SIZE = 256;

const double INIT_DELAY = 1;

enum GameStatus {
    GAME_STOP = 0,
    GAME_RUNNING,
    GAME_OVER,
    GAME_WON,
};

class Game {
    SDL_Renderer *renderer;
    SDL_Event *event;
    Painter *painter;
    Gallery *gallery;

    bool roundWon;
    int width, height;
    int score, round;
    GameStatus status;

    Entity background;
    int scrolling = 0;

    Gundam gundam;

    vector<Chicken*> chickens;
    int numberOfAliveChicken;
    Chicken *topChicken, *bottomChicken, *leftChicken, *rightChicken;
    vector<Bullet*>chickenBullets;
    ChickenMoveState chickenMoveState;

    chrono::system_clock::time_point initStart, initEnd;

    vector<int> killedChickenCount;
    set<Upgrade*> upgrades;
    deque<Entity*> explosions;

public:
    Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height);

    int getWidth() const {
        return width;
    }
    int getHeight() const {
        return height;
    }
    int getGameStatus() const {
        return status;
    }

    void setGameStatus(GameStatus newStatus);
    void init();
    void handleGameEvent();
    void process();

    void dropUpgrade(EntityType type);
    void addExplosion(SDL_Rect rect);
    void gundamDead();
};

#endif // GAME_H_INCLUDED

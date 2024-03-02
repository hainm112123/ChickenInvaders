#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../init.h"
#include "../gundam/gundam.h"
#include "../Painter.h"
#include "../enemy/chicken.h"

const int NUMBER_OF_CHICKEN = 30;
const int NUMBER_OF_CHICKEN_PER_ROW = 10;
const int MINI_BOSS_ROUND = 3;
const int BOSS_ROUND = 4;

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

    Gundam gundam;

    set<Chicken*> chickens;
    Chicken *topChicken, *bottomChicken, *leftChicken, *rightChicken;
    vector<Bullet*>chickenBullets;
    EnemyMoveState enemyMoveState;

    chrono::system_clock::time_point initStart, initEnd;

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

    void handleChicken();
};

#endif // GAME_H_INCLUDED

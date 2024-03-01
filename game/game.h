#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <set>
#include <chrono>
#include <thread>
#include <ctime>

#include "../init.h"
#include "../gundam/gundam.h"
#include "../Painter.h"
#include "../enemy/chicken.h"

#define CLOCK_NOW chrono::system_clock::now

const int NUMBER_OF_CHICKEN = 3;
const int NUMBER_OF_CHICKEN_PER_ROW = 3;
const int MINI_BOSS_ROUND = 3;
const int BOSS_ROUND = 4;

const double INIT_DELAY = 2;

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

    bool roundWon;
    int width, height;
    int score, round;
    GameStatus status;
    Gundam gundam;
    set<Chicken*> chickens;
    vector<Bullet*>chickenBullets;

    chrono::system_clock::time_point initStart, initEnd;

public:
    Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height);

    int getWidth() const {
        return width;
    }
    int getHeight() const {
        return height;
    }

    bool isGameRunning() const {
        return status == GAME_RUNNING;
    }

    void setGameStatus(GameStatus newStatus);
    void init();
    void handleEvent();
    void process();

    void handleChicken();
};

#endif // GAME_H_INCLUDED

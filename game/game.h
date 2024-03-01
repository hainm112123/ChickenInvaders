#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <set>

#include "../init.h"
#include "../gundam/gundam.h"
#include "../Painter.h"
#include "../enemy/chicken.h"

const int NUMBER_OF_CHICKEN = 30;
const int NUMBER_OF_CHICKEN_PER_ROW = 10;

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

    int width, height;
    int score;
    GameStatus status;
    Gundam gundam;
    set<Chicken*> chickens;
    vector<Bullet*>chickenBullets;

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

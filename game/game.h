#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../init.h"
#include "../gundam/gundam.h"
#include "../Painter.h"
#include "../enemy/chicken.h"
#include "../gundam/upgrade.h"
#include "../enemy/rock.h"
#include "text.h"

const int NUMBER_OF_CHICKEN = 30;
const int NUMBER_OF_CHICKEN_PER_ROW = 10;
const int ROCK_FALL_WAVE = 5;
const int ROCK_SIDE_WAVE = 15;
const int ROCK_WAVE_FRAME = 100;

const int ROCK_FALL_ROUND = 5;
const int ROCK_SIDE_ROUND = 3;
const int MINI_BOSS_ROUND = 4;
const int BOSS_ROUND = 6;

const int UPGRADE_SPEED = 1;
const int SCREEN_SPEED = 2;
const int BG_SIZE = 256;

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
    Gallery *gallery;
    Media *media;
    TTF_Font *fontMenu, *fontGame;

    bool roundWon;
    int width, height;
    int score, round;
    GameStatus status;
    Entity background;
    int scrolling = 0;
    int frame = 0;
    chrono::system_clock::time_point initStart, initEnd;

    Gundam gundam;

    vector<Chicken*> chickens;
    int numberOfAliveChicken;
    Chicken *topChicken, *bottomChicken, *leftChicken, *rightChicken;
    vector<Bullet*>chickenBullets;
    ChickenMoveState chickenMoveState;
    vector<int> killedChickenCount;
    set<Rock*> rocks;

    set<Upgrade*> upgrades;
    deque<Entity*> explosions;

public:
    Game(SDL_Renderer *_renderer, SDL_Event *_event, Painter *_painter, int _width, int _height);
    ~Game();

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
    void load();
    void init();
    void renderMenu();
    void handleGameEvent();
    void process();
    void quit();

    void setRoundWon();
    void dropUpgrade(EntityType type);
    void addExplosion(SDL_Rect rect);
    void gundamDead();

};

#endif // GAME_H_INCLUDED

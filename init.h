#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <set>
#include <vector>
#include <assert.h>
#include <chrono>
#include <thread>
#include <ctime>

#include "SDL.h"
#include "SDL_image.h"
#include "Painter.h"
#include "deque"

#define CLOCK_NOW chrono::system_clock::now

using namespace std;

typedef chrono::system_clock::time_point Time;
typedef chrono::duration<double> ElapsedTime;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;
const int GAME_WIDTH = 1200;
const int GAME_HEIGHT = 4096;

const int NUMBER_OF_EXPLOSION_PIC = 8;
const int FRAME_PER_PICTURE = 5;

int Rand(int l, int r);

enum EntityType {
    GUNDAM = 0,
    BACKGROUND,
    CHICKEN,
    CHICKEN_BOSS,
    BULLET,
    LEVEL_UP,
    NEW_WEAPON,
    EXPLOSION,
    ROCK
};

struct ChickenMoveState {
    bool goLeft, goRight, goUp, goDown;
};

class Entity {
    EntityType type;
    SDL_Rect rect;
    Texture texture;
    int step_x = 0, step_y = 0;
    int frame = 0;

public:
    bool operator < (const Entity &other) const {
        return (rect.x < other.rect.x) || (rect.x == other.rect.x && rect.y < other.rect.y);
    }

    Entity();
    Entity(EntityType _type, SDL_Rect _rect = {0, 0, 0, 0}, Texture _texture = {NULL, 0, 0});

    int getX() const {
        return rect.x;
    }
    int getY() const {
        return rect.y;
    }
    int getW() const {
        return rect.w;
    }
    int getH() const {
        return rect.h;
    }
    SDL_Rect getRect() const {
        return rect;
    }
    EntityType getType() const {
        return type;
    }
    int getFrame() const {
        return frame;
    }

    void setStep(int _step_x, int _step_y);
    void updateStep(int det_x, int det_y);

    bool isInsideScreen();
    void _move(bool isInsideScreen = false);
    bool collisionWith(const Entity &entity);

    void render(SDL_Renderer *renderer, int arg = 0);
    void setTexture(Texture _texture);

};

class Gallery {
    Painter *painter;
public:
    vector<vector<Texture>> gundamWeapons;
    vector<Texture> chickens, eggs, gundams, newWeapons;
    Texture laser, rock, background, levelUp, expolosion;

    Gallery(Painter *_painter);
    ~Gallery();

    void loadGamePictures();
};

#endif // INIT_H_INCLUDED

#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <set>
#include <vector>
#include <assert.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <deque>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Painter.h"

#define CLOCK_NOW chrono::system_clock::now
#define _size(x) (int((x).size()))

using namespace std;

typedef chrono::system_clock::time_point Time;
typedef chrono::duration<double> ElapsedTime;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 4096;

const int NUMBER_OF_EXPLOSION_PIC = 8;
const int FRAME_PER_PICTURE = 5;
const int FRAME_PER_PICTURE_MORE = 15;

const int GUNDAM_LASER_WIDTH = 80;
const int GUNDAM_LASER_HIGHT = 682;
const int GUNDAM_LASER_DAMAGE = 1;

int Rand(int l, int r);

enum EntityType {
    GUNDAM = 0,
    BACKGROUND,
    CHICKEN,
    CHICKEN_BOSS,
    BULLET,
    LASER,
    LEVEL_UP,
    NEW_WEAPON,
    EXPLOSION,
    ROCK,
    MENU,
    TEXT,
    SHIELD
};

struct ChickenMoveState {
    bool goLeft, goRight, goUp, goDown;
};

class Entity {
protected:
    EntityType type;
    SDL_Rect rect;
    Texture texture;
    vector<Texture> textures;
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

    void setRect(int x, int y);
    void setRect(SDL_Rect _rect);
    void setStep(int _step_x, int _step_y);
    void updateStep(int det_x, int det_y);

    bool isInsideScreen();
    void _move(bool isInsideScreen = false);
    bool collisionWith(const Entity &entity);

    void render(SDL_Renderer *renderer, int arg = 0);
    void setTexture(Texture _texture, bool isSetRect = false);
    void setTextures(vector< Texture > _textures);
};

class Gallery {
    Painter *painter;
public:
    vector<vector<Texture>> gundamWeapons, chickens;
    vector<Texture> eggs, gundams, newWeapons, rocks;
    Texture laser, background, levelUp, expolosion, menu, shield, menu_settings;

    Gallery(Painter *_painter);
    ~Gallery();

    void loadGamePictures();
};

class Media {
public:
    vector<Mix_Chunk*> bullets, explosions, chickens;
    Mix_Chunk *upgrade, *bulletRock, *laser;
    Mix_Music *music;

    Media();
    ~Media();

    void loadMedia();
};

class Timer {
    Time st, ed;
    double duration = 0;
    bool active = false;
public:
    Timer(double duration = 0);

    void setDuration(double _duration);
    void startCountdown();
    bool timeIsUp();
};

#endif // INIT_H_INCLUDED

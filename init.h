#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <set>
#include <vector>
#include <assert.h>

#include "SDL.h"
#include "SDL_image.h"
#include "Painter.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 4096;

enum WeaponType {
    GUNDAM_BLASTER = 0,
    GUNDAM_BORON,
    GUNDAM_NEUTRON,
    GUNDAM_LASER,

    CHICKEN_EGG,
};

struct EnemyMoveState {
    int goLeft, goRight, goUp, goDown;
};

class Entity {
    SDL_Rect rect;
    SDL_Texture *texture;
    int step_x = 0, step_y = 0;

public:
    Entity();
    Entity(SDL_Rect _rect);

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

    void setStep(int _step_x, int _step_y);
    void updateStep(int det_x, int det_y);

    void _move(bool isInsideScreen = false);
    bool collisionWith(const Entity &entity);

    void render(SDL_Renderer *renderer);

};

#endif // INIT_H_INCLUDED

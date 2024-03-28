#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "common.h"
#include "time.h"

typedef pair<double, double> Position;

const int NUMBER_OF_EXPLOSION_PIC = 4;
const int NUMBER_OF_BIG_EXPLOSION_PIC = 8;
const int NUMBER_OF_TELEPORT_PIC = 12;
const double SECOND_PER_PICTURE = 0.06;
const double SECOND_PER_PICTURE_FASTER = 0.04;
const double SECOND_PER_PICTURE_LONGER = 0.1;

enum EntityType {
    GUNDAM = 0,
    BACKGROUND,
    CHICKEN,
    BULLET,
    ROCKET,
    LASER,
    LEVEL_UP,
    NEW_WEAPON,
    EXPLOSION,
    BIG_EXPLOSION,
    TELEPORT,
    ROCK,
    MENU,
    TEXT,
    SHIELD,
    HEART,
    GUNDAM_STATE,
    FRIED_CHICKEN,
    BOSS_HEALTH_BAR,
    PAUSE_MENU_BUTTON
};

class Entity {
protected:
    EntityType type;
    SDL_Rect rect;
    Texture texture;
    vector<Texture> textures;
    double step_x = 0, step_y = 0;
    double x, y;
    double currentTime = 0;

public:
    bool operator < (const Entity &other) const {
        return (rect.x < other.rect.x) || (rect.x == other.rect.x && rect.y < other.rect.y);
    }

    Entity();
    Entity(EntityType _type, SDL_Rect _rect = {0, 0, 0, 0}, Texture _texture = {NULL, 0, 0});
    ~Entity();

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
    double CurrentTime() const {
        return currentTime;
    }
    double get_act_x() const {
        return x;
    }
    double get_act_y() const {
        return y;
    }
    pair<double, double> getStep() const {
        return make_pair(step_x, step_y);
    }
    Position NextPosition() {
        return Position(x + step_x, y + step_y);
    }

    void setPosition(double _x, double _y);
    void setRect(int _x, int _y);
    void setRect(SDL_Rect _rect);
    void setStep(double _step_x, double _step_y);
    void updateStep(double det_x, double det_y);
    void resetTime();

    bool isInsideScreen();
    void _move(bool isInsideScreen = false);
    bool collisionWith(const Entity *entity);
    bool collisionWith(const Entity &entity);

    void render(SDL_Renderer *renderer, int arg = 0);
    void setTexture(Texture _texture, bool isSetRect = false);
    void setTextures(vector< Texture > _textures);
};

#endif // ENTITY_H_INCLUDED

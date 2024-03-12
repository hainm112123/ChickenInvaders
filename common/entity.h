#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "common.h"

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
    SHIELD,
    HEART
};

class Entity {
protected:
    EntityType type;
    SDL_Rect rect;
    Texture texture;
    vector<Texture> textures;
    double step_x = 0, step_y = 0;
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

#endif // ENTITY_H_INCLUDED

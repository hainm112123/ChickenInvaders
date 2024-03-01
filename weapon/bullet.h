#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "../init.h"
#include "weapon.h"

const int GUNDAM_BLASTER_SPEED = 8;
const int GUNDAM_BLASTER_DAMAGE = 5;

const int BLASTER_HEIGHT = 15;
const int BLASTER_WIDTH = 10;

class Bullet {
    Entity entity;
    WeaponType type;
    bool isMove;
public:
    Bullet();
    Bullet(Entity _entity, WeaponType _type = GUNDAM_BLASTER);

    bool getIsMove() const {
        return isMove;
    }
    void setIsMove(const bool _isMove);
    Entity* getEntity() {
        return &entity;
    }
    void setEntity(SDL_Rect rect, int speed);

    void render(SDL_Renderer *renderer);
    void handleGundamBullet();
    void handleEnemyBullet();
};

#endif // BULLET_H_INCLUDED

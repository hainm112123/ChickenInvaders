#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "../init.h"

enum WeaponType {
    GUNDAM_BLASTER = 0,
    GUNDAM_BORON,
    GUNDAM_NEUTRON,
    GUNDAM_LASER,

    CHICKEN_EGG,
};

const int GUNDAM_BULLET_DAMAGE[] = {50, 30, 80};
const int GUNDAM_BULLET_SPEED[] = {8, 12, 5};
const int GUNDAM_BULLET_UPGRADE[] = {30, 20, 50};

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
    void setEntity(SDL_Rect rect, int speed, Texture texture);

    void render(SDL_Renderer *renderer);
    void handleGundamBullet();
    void handleEnemyBullet();
};

#endif // BULLET_H_INCLUDED

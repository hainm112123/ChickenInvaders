#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

enum WeaponType {
    GUNDAM_BLASTER = 0,
    GUNDAM_BORON,
    GUNDAM_NEUTRON,
    GUNDAM_LASER,

    CHICKEN_EGG,
};

const int GUNDAM_BULLET_DAMAGE[] = {56, 40, 80};
const double GUNDAM_BULLET_SPEED[] = {800, 1500, 600};
const int GUNDAM_BULLET_UPGRADE[] = {25, 16, 49};

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
    void setEntity(SDL_Rect rect, double speed, Texture texture);

    void render(SDL_Renderer *renderer);
    void handleGundamBullet();
    void handleEnemyBullet();
};

#endif // BULLET_H_INCLUDED

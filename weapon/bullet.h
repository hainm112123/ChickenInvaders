#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

enum WeaponType {
    GUNDAM_BLASTER = 0,
    GUNDAM_BORON,
    GUNDAM_NEUTRON,
    GUNDAM_AUTO_AIM,
    GUNDAM_LASER,

    CHICKEN_EGG,
};

const double GUNDAM_BULLET_DAMAGE[] = {56, 40, 81, 144};
const double GUNDAM_BULLET_SPEED[] = {800, 1500, 600, 196};
const double GUNDAM_BULLET_UPGRADE[] = {25, 16, 49, 64};

const double MAX_AUTO_AIM_DISTANCE = 300;

class Bullet {
    Entity entity;
    WeaponType type;
    bool isMove;
    double speed;
public:
    Bullet(WeaponType _type);
    Bullet(Entity _entity, WeaponType _type);

    bool getIsMove() const {
        return isMove;
    }
    void setIsMove(const bool _isMove);
    Entity* getEntity() {
        return &entity;
    }
    WeaponType Type() const {
        return type;
    }

    void setEntity(SDL_Rect rect, double _speed, Texture texture);

    void render(SDL_Renderer *renderer);
    void handleGundamBullet(double target_x = -oo, double target_y = -oo);
    bool handleEnemyBullet(bool is_boss = false);
};

#endif // BULLET_H_INCLUDED

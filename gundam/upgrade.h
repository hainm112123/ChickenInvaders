#ifndef UPGRADE_H_INCLUDED
#define UPGRADE_H_INCLUDED

#include "../common/common.h"
#include "../common/entity.h"

enum UpgradeType {
    UPGRADE_ADD_BLASTER = 0,
    UPGRADE_ADD_BORON,
    UPGRADE_ADD_NEUTRON,
    UPGRADE_ADD_LASER,
    UPGRADE_LEVEL_UP,
};

const int NUMBER_OF_UPGRADE = 5;
const int UPGRADE_SPEED = 200;

class Upgrade {
    Entity entity;
    UpgradeType type;

public:
    Upgrade(UpgradeType _type, SDL_Rect rect = {0, 0, 0, 0});

    void _move();
    void render(SDL_Renderer *renderer);
    bool isInsideScreen();

    UpgradeType getType() const {
        return type;
    }

    Entity* getEntity() {
        return &entity;
    }
};

#endif // UPGRADE_H_INCLUDED

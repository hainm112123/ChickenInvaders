#include "upgrade.h"

Upgrade::Upgrade(UpgradeType _type, SDL_Rect rect): type(_type) {
    entity = Entity(type == UPGRADE_LEVEL_UP ? LEVEL_UP : NEW_WEAPON, rect);
}

void Upgrade::_move() {
    entity._move();
}

void Upgrade::render(SDL_Renderer *renderer) {
    entity.render(renderer);
}

bool Upgrade::isInsideScreen() {
    return entity.getX() >= 0 && entity.getX() <= SCREEN_WIDTH && entity.getY() <= SCREEN_HEIGHT + 10;
}

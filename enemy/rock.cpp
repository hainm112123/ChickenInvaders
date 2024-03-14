#include "rock.h"

void Rock::setActive(bool _active) {
    active = _active;
}
void Rock::setHP(double _hp) {
    hp = _hp;
}

bool Rock::receiveDamage(double dmg) {
    hp -= dmg;
    if (hp <= 0) setActive(false);
    return hp > 0;
}

void Rock::handleMove() {
    if (!isActive()) return;
    _move();
    if (getX() > SCREEN_WIDTH + getW() + 10 || getY() > SCREEN_HEIGHT + getH() + 10) setActive(false);
}

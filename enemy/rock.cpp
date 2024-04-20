#include "rock.h"

Rock::~Rock() {
    for (Entity *spark: sparks) delete(spark);
    sparks.clear();
}

void Rock::setActive(bool _active) {
    active = _active;
}
void Rock::setHP(double _hp) {
    hp = _hp;
}

bool Rock::receiveDamage(double dmg) {
    Entity *spark = new Entity(SPARK, {rect.x + rect.w/2 - SPARK_WIDTH/2, rect.y + rect.h/2 - SPARK_HEIGHT/2, SPARK_WIDTH, SPARK_HEIGHT}, Gallery::Instance()->spark);
    sparks.push_back(spark);
    hp -= dmg;
    if (hp <= 0) setActive(false);
    return hp > 0;
}

void Rock::handleMove() {
    if (!isActive()) return;
    _move();
    if (getX() > SCREEN_WIDTH + getW() + 10 || getY() > SCREEN_HEIGHT + getH() + 10) setActive(false);
}

void Rock::handleState(SDL_Renderer *renderer) {
    while (!sparks.empty() && sparks.front()->CurrentTime() >= SECOND_PER_PICTURE * 10) {
        Entity *spark = sparks.front();
        sparks.pop_front();
        delete(spark);
    }
    for (auto *spark: sparks) {
//        cout << "spark render\n";
        spark->setRect(rect.x + rect.w/2 - spark->getW()/2, rect.y + rect.h/2 - spark->getH()/2);
        spark->render(renderer);
    }
}

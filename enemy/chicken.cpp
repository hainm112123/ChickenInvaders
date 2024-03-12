#include "chicken.h"

Chicken::Chicken(int offsetX, int offsetY, int _level, int NG) {
    level = _level;
    int width = CHICKEN_WIDTH[level];
    int height = CHICKEN_HEIGHT[level];
    int distance = CHICKENS_DISTANCE[level];
    entity = Entity((level == 0 ? CHICKEN : CHICKEN_BOSS), {offsetX * (width + distance), 50 + offsetY * (height + distance), width, height});

    hp = CHICKEN_HP[level] + CHICKEN_HP_UPGRADE[level] * NG;
    speed = CHICKEN_SPEED[level] + NG_CHICKEN_SPEED * NG;
    bulletWidth = CHICKEN_EGG_WIDTH[level];
    bulletHeight = CHICKEN_EGG_HEIGHT[level];
    bulletSpeed = Rand(MIN_CHICKEN_EGG_SPEED[level], MAX_CHICKEN_EGG_SPEED[level]) + NG * NG_CHICKEN_EGG_SPEED;
}

void Chicken::setLastBullet(Time _lastBullet) {
    lastBullet = _lastBullet;
}

void Chicken::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 0);
    entity.render(renderer);
}

void Chicken::_move(double step_x, double step_y) {
    entity.setStep(step_x, step_y);
    entity._move();
}

bool Chicken::receiveDamage(int dmg) {
//    cout << hp << " " << dmg << "\n";
    hp -= dmg;
    return hp > 0;
}

void Chicken::addBullet(Bullet *bullet, Gallery *gallery) {
    Texture texture = gallery->eggs[level];
    bullet->setEntity({entity.getX() + entity.getW()/2 - bulletWidth/2, entity.getY() + entity.getH(), texture.w, texture.h}, bulletSpeed, texture);
    bullet->setIsMove(true);
    bullets.insert(bullet);
}

void Chicken::removeBullet(Bullet *bullet) {
    assert(bullets.find(bullet) != bullets.end());
    bullets.erase(bullet);
}

void Chicken::handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets) {
    for (Bullet *bullet: bullets) {
        if (bullet->getIsMove()) {
            bullet->render(renderer);
            bullet->handleEnemyBullet();
        }
        else {
            gameEnemyBullets.push_back(bullet);
            removeBullet(bullet);
        }
    }
}

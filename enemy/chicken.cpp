#include "chicken.h"

Chicken::Chicken(int offsetX, int offsetY, int _level) {
    level = _level;
    int width = CHICKEN_WIDTH[level];
    int height = CHICKEN_HEIGHT[level];
    int distance = CHICKENS_DISTANCE[level];
    entity = Entity({offsetX * (width + distance), offsetY * (height + distance), width, height});

    hp = CHICKEN_HP[level];
    speed = CHICKEN_SPEED[level];
    bulletWidth = CHICKEN_EGG_WIDTH[level];
    bulletHeight = CHICKEN_EGG_HEIGHT[level];
    bulletSpeed = CHICKEN_EGG_SPEED[level];

    moveState = {0, 1, 0, !level};
}

void Chicken::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 0);
    entity.render(renderer);
}

void Chicken::_move() {
    int step_x = 0, step_y = 0;
    if (moveState.goLeft) step_x = -speed;
    if (moveState.goRight) step_x = speed;
    if (moveState.goUp) step_y = -speed;
    if (moveState.goDown) step_y = speed;
    entity.setStep(step_x, step_y);
    entity._move();
    if (entity.getY() > SCREEN_HEIGHT) {
        moveState.goDown = 0;
        moveState.goUp = 1;
    }
    if (entity.getY() < -entity.getH()) {
        moveState.goDown = 1;
        moveState.goUp = 0;
    }
    if (entity.getX() > SCREEN_WIDTH + entity.getW()) {
        moveState.goRight = 0;
        moveState.goLeft = 1;
    }
    if (entity.getX() < -entity.getW()) {
        moveState.goRight = 1;
        moveState.goLeft = 0;
    }
}

bool Chicken::receiveDamage(int dmg) {
//    cout << hp << " " << dmg << "\n";
    hp -= dmg;
    return hp > 0;
}

void Chicken::addBullet(Bullet *bullet) {
    bullet->setEntity({entity.getX() + entity.getW()/2 - bulletWidth/2, entity.getY() + entity.getH(), bulletWidth, bulletHeight}, bulletSpeed);
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

#include "chicken.h"

Chicken::Chicken(int offsetX, int offsetY) {
    entity = Entity({offsetX, offsetY, CHICKEN_WIDTH, CHICKEN_HEIGHT});
    hp = CHICKEN_HP;
    moveState = {0, 1, 0, 1};
    isBoss = false;
}

void Chicken::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 0);
    entity.render(renderer);
}

void Chicken::_move() {
    int step_x = 0, step_y = 0;
    if (moveState.goLeft) step_x = -CHICKEN_SPEED;
    if (moveState.goRight) step_x = CHICKEN_SPEED;
    if (moveState.goUp) step_y = -CHICKEN_SPEED;
    if (moveState.goDown) step_y = CHICKEN_SPEED;
    entity.setStep(step_x, step_y);
    entity._move();
    if (entity.getY() > SCREEN_HEIGHT) {
        moveState.goDown = 0;
        moveState.goUp = 1;
    }
    if (entity.getY() < -CHICKEN_HEIGHT) {
        moveState.goDown = 1;
        moveState.goUp = 0;
    }
    if (entity.getX() > SCREEN_WIDTH + CHICKEN_WIDTH) {
        moveState.goRight = 0;
        moveState.goLeft = 1;
    }
    if (entity.getX() < -CHICKEN_WIDTH) {
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
    bullet->setEntity({entity.getX() + entity.getW()/2 - CHICKEN_EGG_WIDTH/2, entity.getY() + entity.getH(), CHICKEN_EGG_WIDTH, CHICKEN_EGG_HEIGHT}, CHICKEN_EGG_SPEED);
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

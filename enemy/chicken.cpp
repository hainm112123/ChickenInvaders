#include "chicken.h"

Chicken::Chicken(int offsetX, int offsetY, ChickenType _type, int game_difficulty) {
    type = _type;
    int width = CHICKEN_WIDTH[type];
    int height = CHICKEN_HEIGHT[type];
    int distance = CHICKENS_DISTANCE[type];
    entity = Entity(CHICKEN, {offsetX * (width + distance), 20 + offsetY * (height + distance), width, height});

    hp = CHICKEN_HP[type] + CHICKEN_HP_UPGRADE[type] * game_difficulty;
    speed = CHICKEN_SPEED[type] + NG_CHICKEN_SPEED * game_difficulty;
    bulletWidth = CHICKEN_EGG_WIDTH[type];
    bulletHeight = CHICKEN_EGG_HEIGHT[type];
    bulletSpeed = Rand(MIN_CHICKEN_EGG_SPEED[type], MAX_CHICKEN_EGG_SPEED[type]) + game_difficulty * NG_CHICKEN_EGG_SPEED;

    moveState = {0, 1, 0, (type == CHICKEN_SMALL)};
    bulletTimer.setDuration(BULLET_DELAY);
}

Chicken::~Chicken() {
    for (Bullet *bullet: bullets) delete(bullet);
    bullets.clear();
}

void Chicken::render(SDL_Renderer *renderer) {
//    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 0);
    entity.render(renderer);
}

void Chicken::setMoveState(ChickenMoveState _moveState) {
    moveState = _moveState;
}

void Chicken::_move() {
    double step_x = 0, step_y = 0;
    if (moveState.goLeft) step_x = -speed;
    if (moveState.goRight) step_x = speed;
    if (moveState.goUp) step_y = -speed;
    if (moveState.goDown) step_y = speed;
//    cout << step_x << " " << step_y << "\n";
    entity.setStep(step_x, step_y);
    entity._move();
}

bool Chicken::receiveDamage(double dmg) {
//    cout << hp << " " << dmg << "\n";
    hp -= dmg;
    return hp > 0;
}

void Chicken::addBullet(Bullet *bullet) {
    Texture texture = Gallery::Instance()->eggs[type];
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

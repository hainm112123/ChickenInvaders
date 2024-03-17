#include "chicken.h"

Chicken::Chicken(int offsetX, int offsetY, int _level, int game_difficulty) {
    level = _level;
    int width = CHICKEN_WIDTH[level];
    int height = CHICKEN_HEIGHT[level];
    int distance = CHICKENS_DISTANCE[level];
    entity = Entity((level == 0 ? CHICKEN : CHICKEN_BOSS), {offsetX * (width + distance), 20 + offsetY * (height + distance), width, height});

    hp = CHICKEN_HP[level] + CHICKEN_HP_UPGRADE[level] * game_difficulty;
    speed = CHICKEN_SPEED[level] + NG_CHICKEN_SPEED * game_difficulty;
    bulletWidth = CHICKEN_EGG_WIDTH[level];
    bulletHeight = CHICKEN_EGG_HEIGHT[level];
    bulletSpeed = Rand(MIN_CHICKEN_EGG_SPEED[level], MAX_CHICKEN_EGG_SPEED[level]) + game_difficulty * NG_CHICKEN_EGG_SPEED;

    moveState = {0, 1, 0, !level};
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
    Texture texture = Gallery::Instance()->eggs[level];
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

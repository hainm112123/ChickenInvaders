#include "chicken.h"

Chicken::Chicken(ChickenType _type, ChickenMoveType _moveType, int game_difficulty, vector<int> args) {
    type = _type;
    moveType = _moveType;
    int width = CHICKEN_WIDTH[type];
    int height = CHICKEN_HEIGHT[type];
    int chicken_distance = CHICKENS_DISTANCE[type];

    if (moveType == CHICKEN_BASIC_MOVE) {
        assert(_size(args) >= 2);
        entity = Entity(CHICKEN, {args[0] * (width + chicken_distance), 20 + args[1] * (height + chicken_distance), width, height});
        moveState = {0, 1, 0, (type == CHICKEN_SMALL)};
        speed = CHICKEN_SPEED[type] + NG_CHICKEN_SPEED * game_difficulty;
    }
    else {
        assert(_size(args) >= 2);
        entity = Entity(CHICKEN, {0, 0, width, height});
        circular_distance = args[0];
        angle = args[1];
        entity.setPosition(CIRCULAR_CENTER_X + circular_distance * cos(to_radian(angle)), CIRCULAR_CENTER_Y + circular_distance * sin(to_radian(angle)));
        speed = (CHICKEN_SPEED[type] + NG_CHICKEN_SPEED * game_difficulty) / 2;
    }

    hp = CHICKEN_HP[type] + CHICKEN_HP_UPGRADE[type] * game_difficulty;
    bulletWidth = CHICKEN_EGG_WIDTH[type];
    bulletHeight = CHICKEN_EGG_HEIGHT[type];
    bulletSpeed = Rand(MIN_CHICKEN_EGG_SPEED[type], MAX_CHICKEN_EGG_SPEED[type]) + game_difficulty * NG_CHICKEN_EGG_SPEED;

    bulletTimer.setDuration(BULLET_DELAY);
    teleportCooldown.setDuration(CHICKEN_TELEPORT_COOLDOWN);
    teleportDuration.setDuration(CHICKEN_TELEPORT_DURATION);
}

Chicken::~Chicken() {
    for (Bullet *bullet: bullets) delete(bullet);
    bullets.clear();
    for (Entity *explosion: explosions) delete(explosion);
    explosions.clear();
}

void Chicken::render(SDL_Renderer *renderer) {
    entity.render(renderer);
}

void Chicken::setMoveState(ChickenMoveState _moveState) {
    moveState = _moveState;
}

void Chicken::setOnTeleport(bool val) {
    onTeleport = val;
}

void Chicken::_move() {
    if (moveType == CHICKEN_BASIC_MOVE) {
        double step_x = 0, step_y = 0;
        if (moveState.goLeft) step_x = -speed;
        if (moveState.goRight) step_x = speed;
        if (moveState.goUp) step_y = -speed;
        if (moveState.goDown) step_y = speed;
    //    cout << step_x << " " << step_y << "\n";
        entity.setStep(step_x, step_y);
        entity._move();
    }
    else {
        angle += double(30) * TimeManager::Instance()->getElapsedTime();
        if (angle >= 360) angle -= 360;
        circular_distance -= speed * TimeManager::Instance()->getElapsedTime() * direction;
        if (circular_distance > CHICKEN_CIRCULAR_DISTANCE_MAX || circular_distance < CHICKEN_CIRCULAR_DISTANCE_MIN) {
            direction *= -1;
        }
        entity.setPosition(CIRCULAR_CENTER_X + circular_distance * cos(to_radian(angle)), CIRCULAR_CENTER_Y + circular_distance * sin(to_radian(angle)));
    }
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

void Chicken::removeBullet(Bullet *bullet, vector<Bullet*> &gameEnemyBullets) {
    gameEnemyBullets.push_back(bullet);
    assert(bullets.find(bullet) != bullets.end());
    bullets.erase(bullet);
}

void Chicken::handleBullet(SDL_Renderer *renderer, vector<Bullet*> &gameEnemyBullets) {
    for (Bullet *bullet: bullets) {
        if (bullet->getIsMove()) {
            bullet->render(renderer);
            bool tmp = bullet->handleEnemyBullet(type == CHICKEN_BOSS);
            if (type == CHICKEN_BOSS && tmp) {
                SDL_Rect rect = bullet->getEntity()->getRect();
                rect.x -= 50;
                rect.y -= 50;
                rect.w = CHICKEN_BOMB_EXPLOSION_WIDTH;
                rect.h = CHICKEN_BOMB_EXPLOSION_HEIGHT;
                explosions.push_back(new Entity(EXPLOSION, rect, Gallery::Instance()->expolosions[1]));
            }
        }
        else {
            removeBullet(bullet, gameEnemyBullets);
        }
    }
}

void Chicken::handleExplosion(SDL_Renderer *renderer) {
    while (!explosions.empty() && explosions.front()->CurrentTime() >= SECOND_PER_PICTURE * NUMBER_OF_EXPLOSION_PIC) {
        Entity *explosion = explosions.front();
        explosions.pop_front();
        delete(explosion);
    }
    for (auto *explosion: explosions) explosion->render(renderer);
}

bool Chicken::checkBulletCollision(Entity *other) {
    for (Bullet *bullet: bullets) {
        if (bullet->getEntity()->collisionWith(other)) return true;
    }
    for (Entity *explosion: explosions) {
        if (explosion->collisionWith(other)) return true;
    }
    return false;
}

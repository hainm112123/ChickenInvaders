#include "bullet.h"

Bullet::Bullet() {
    isMove = false;
    type = GUNDAM_BLASTER;
}
Bullet::Bullet(Entity _entity, WeaponType _type): entity(_entity), type(_type) {
    isMove = false;
}

void Bullet::setIsMove(const bool _isMove) {
    isMove = _isMove;
}
void Bullet::setEntity(SDL_Rect rect, int speed, Texture texture) {
    entity = Entity(BULLET, rect);
    entity.setStep(0, speed);
    entity.setTexture(texture);
//    cout << entity.getX() << " " << entity.getY() << " " << speed << "\n";
}

void Bullet::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, RED_COLOR.r, RED_COLOR.g, RED_COLOR.b, 0);
//    cout << entity.getX() << " " << entity.getY() << "\n";
    entity.render(renderer);
}

void Bullet::handleGundamBullet() {
    entity._move();
    if (entity.getY() < -entity.getH() - 100) {
        setIsMove(false);
    }
}

void Bullet::handleEnemyBullet() {
    entity._move();
    if (entity.getY() > SCREEN_HEIGHT + entity.getH() + 100) {
        setIsMove(false);
    }
}

